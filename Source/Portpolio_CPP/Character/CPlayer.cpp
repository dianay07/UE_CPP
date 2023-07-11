#include "Character/CPlayer.h"

#include "CEnemy.h"
#include "Camera/CameraAnim.h"
#include "Camera/CameraComponent.h"
#include "Component/CameraControlComponent.h"
#include "Component/CEquipComponent.h"
#include "Component/CJobComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CTargetComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/CUI_TargetInfo.h"

#include "Kismet/GameplayStatics.h"

ACPlayer::ACPlayer()
{
	// ACCharacterBase 세팅
	{
		// 메쉬
		ConstructorHelpers::FObjectFinder<USkeletalMesh>
			SK_Mesh(TEXT("SkeletalMesh'/Game/Fallen_Knight/Mesh/SK_Fallen_Knight.SK_Fallen_Knight'"));
		if (SK_Mesh.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(SK_Mesh.Object);
		}
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

		TSubclassOf<UAnimInstance> instance;
		instance = ConstructorHelpers::FClassFinder<UAnimInstance>(TEXT("AnimBlueprint'/Game/01_Player/ABP_Player.ABP_Player_C'")).Class;
		GetMesh()->SetAnimClass(instance);
	}

	// CPlayer 세팅
	{
		// SpringArm
		SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
		SpringArm->SetupAttachment(GetMesh());
		SpringArm->SetRelativeLocation(FVector(-20, 0, 160));
		SpringArm->SetRelativeRotation(FRotator(0, 90.0f, 0));
		SpringArm->TargetArmLength = 400;
		SpringArm->bEnableCameraLag = true;
		SpringArm->bUsePawnControlRotation = true;

		// Camera
		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		Camera->SetupAttachment(SpringArm);
	}

	// Component
	{
		// Camera Controller
		CameraController = CreateDefaultSubobject<UCameraControlComponent>(TEXT("Camera Control"));
		Job = CreateDefaultSubobject<UCJobComponent>(TEXT("Job Component"));
		Equip = CreateDefaultSubobject<UCEquipComponent>(TEXT("Equip Component"));
	}

	// Enemy Targeting Ui
	static ConstructorHelpers::FClassFinder<UCUI_TargetInfo> WB_InfoUI(TEXT("WidgetBlueprint'/Game/07_UI/BP_CUI_TargetInfo.BP_CUI_TargetInfo_C'"));
	if(WB_InfoUI.Succeeded())
		UI_TargetInfoClass = WB_InfoUI.Class;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	UI_TargetInfo = Cast<UCUI_TargetInfo>(CreateWidget(GetWorld(), UI_TargetInfoClass));
	UI_TargetInfo->AddToViewport();
	UI_TargetInfo->SetVisibility(ESlateVisibility::Hidden);

	Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Controller->SetShowMouseCursor(true);

	TestKeyBinding();
}

void ACPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 축 바인딩
	{
		PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
		PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
		PlayerInputComponent->BindAxis("VerticalLook", CameraController, &UCameraControlComponent::OnVerticalLook);
		PlayerInputComponent->BindAxis("HorizontalLook", CameraController, &UCameraControlComponent::OnHorizontalLook);
		PlayerInputComponent->BindAxis("Zoom", CameraController, &UCameraControlComponent::ApplyZoom);
	}

	// 키 바인딩
	{
		PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
		PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::OnJump);
		PlayerInputComponent->BindAction("CameraControl", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::FixedCameraSetting);
		PlayerInputComponent->BindAction("CameraControl", EInputEvent::IE_Released, Movement, &UCMovementComponent::FixedCharacterSetting);

		PlayerInputComponent->BindAction("Targeting_Tab", EInputEvent::IE_Pressed, this, &ACPlayer::TabOnTarget);
		PlayerInputComponent->BindAction("Targeting_Click", EInputEvent::IE_Pressed, this, &ACPlayer::ClickOnTarget);

		// 키 테스팅
		PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Job, &UCJobComponent::UseSkill);

		PlayerInputComponent->BindAction("SwapToWarrior", EInputEvent::IE_Pressed, Job, &UCJobComponent::SetWarrior);
		PlayerInputComponent->BindAction("SwapToDragoon", EInputEvent::IE_Pressed, Job, &UCJobComponent::SetDragoon);
	}
}

void ACPlayer::OnJump()
{
	Jump();
}

void ACPlayer::DisplayTargetInfo(ACCharacterBase* InOther)
{
	// 타겟 컴포넌트에 선택된 타겟이 없으면
	if (IsValid(Target->GetTargetActor()) == false)
		return;

	// UI가 안켜져 있으면 켜주고
	if(UI_TargetInfo->IsVisible() == false)
		UI_TargetInfo->SetVisibility(ESlateVisibility::Visible);

	// 표시될 데이터 설정
	UI_TargetInfo->SetLevelName(Target->GetTargetActor()->GetName());
	UI_TargetInfo->SetLevelText(" LV : 00 ");
}

void ACPlayer::TestKeyBinding()
{
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SwapToWarrior", EKeys::One, true));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SwapToDragoon", EKeys::Two, true));
}

void ACPlayer::OffTargetInfo()
{
	if (UI_TargetInfo->IsVisible() == true)
		UI_TargetInfo->SetVisibility(ESlateVisibility::Hidden);
}

// 지정 없이 타겟팅 실행
void ACPlayer::TabOnTarget()
{
	Target->ToggleTarget();

	if (Target != nullptr)
		DisplayTargetInfo(Target->GetTargetActor());
}

// 타겟이 될 물체 클릭 이벤트
void ACPlayer::ClickOnTarget()
{
	/* 마우스 커서 위치로부터 스크린 라인 트레이스  */
	FVector start;
	FVector direction;
	Controller->DeprojectMousePositionToWorld(start, direction);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *start.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *direction.ToString());

	float distance = 10000.0f;
	FVector end = start + direction * distance;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;	// 히트 가능한 오브젝트 유형 담을 배열
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(Pawn);

	TArray<AActor*> ignores;
	ignores.AddUnique(this);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, end, ObjectTypes, false,
		ignores, EDrawDebugTrace::ForDuration, hitResult, true);

	//UE_LOG(LogTemp, Display, TEXT("%s"), *hitResult.GetActor()->GetName());

	if(Cast<ACEnemy>(hitResult.GetActor()))
	{
		if(GetTarget()->GetTargetActor() == hitResult.GetActor())
		{
			State->SetIsBattle(true);
			Job->OnAutoAttack();
		}

		// hitActor를 CharacterBase로 형변환 후 전달
		ACCharacterBase* character = Cast<ACCharacterBase>(hitResult.GetActor());
		Target->ToggleTarget(character);
		DisplayTargetInfo(character);
	}
}