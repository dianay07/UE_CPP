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
#include "Particles/ParticleSystemComponent.h"
#include "UI/CHUDLayout.h"

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
		CameraController = CreateDefaultSubobject<UCameraControlComponent>(TEXT("Camera Control"));
		Job = CreateDefaultSubobject<UCJobComponent>(TEXT("Job Component"));
		Equip = CreateDefaultSubobject<UCEquipComponent>(TEXT("Equip Component"));
	}

	// Enemy Targeting Ui
	static ConstructorHelpers::FClassFinder<UCUI_TargetInfo> WB_InfoUI(TEXT("WidgetBlueprint'/Game/07_UI/BP_CUI_TargetInfo.BP_CUI_TargetInfo_C'"));
	if(WB_InfoUI.Succeeded())
		UI_TargetInfoClass = WB_InfoUI.Class;

	// HUDLayout
	static ConstructorHelpers::FClassFinder<UCHUDLayout> WB_HUDLayout(TEXT("WidgetBlueprint'/Game/07_UI/BP_CHUDLayout.BP_CHUDLayout_C'"));
	if (WB_HUDLayout.Succeeded())
		UI_HUDLayoutClass = WB_HUDLayout.Class;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	UI_TargetInfo = Cast<UCUI_TargetInfo>(CreateWidget(GetWorld(), UI_TargetInfoClass));
	UI_TargetInfo->AddToViewport();
	UI_TargetInfo->SetVisibility(ESlateVisibility::Hidden);

	// ??????
	UI_HUDLayout = Cast<UCHUDLayout>(CreateWidget(GetWorld(), UI_HUDLayoutClass));
	UI_HUDLayout->AddToViewport();

	Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Controller->SetShowMouseCursor(true);

	TestKeyBinding();
	Job->ChangeJob(EJob::Warrior);
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
		//PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
		//PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::OnJump);
		PlayerInputComponent->BindAction("CameraControl", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::FixedCameraSetting);
		PlayerInputComponent->BindAction("CameraControl", EInputEvent::IE_Released, Movement, &UCMovementComponent::FixedCharacterSetting);

		PlayerInputComponent->BindAction("Targeting_Tab", EInputEvent::IE_Pressed, this, &ACPlayer::TabOnTarget);
		PlayerInputComponent->BindAction("Targeting_Click", EInputEvent::IE_Pressed, this, &ACPlayer::ClickOnTarget);
		PlayerInputComponent->BindAction("Targeting_Click", EInputEvent::IE_DoubleClick, this, &ACPlayer::DoubleClickOnTarget);

		// 단축키 키는 고정 -> Job Component에서 스킬이 바껴야할듯
		PlayerInputComponent->BindAction("Slot1", EInputEvent::IE_Pressed, Job, &UCJobComponent::UseFirstSlot);
		PlayerInputComponent->BindAction("Slot2", EInputEvent::IE_Pressed, Job, &UCJobComponent::UseSecondSlot);
		PlayerInputComponent->BindAction("SubSkill", EInputEvent::IE_Pressed, this, &ACPlayer::OnSubAction);

		// 키 테스트
		PlayerInputComponent->BindAction("SwapToWarrior", EInputEvent::IE_Pressed, Job, &UCJobComponent::SetWarrior);
		PlayerInputComponent->BindAction("SwapToDragoon", EInputEvent::IE_Pressed, Job, &UCJobComponent::SetDragoon);
	}
}

void ACPlayer::OnJump()
{
	Jump();
}

void ACPlayer::DisplayTargetInfo(const ACCharacterBase& InOther)
{
	// UI가 안켜져 있으면 켜주고
	if(UI_TargetInfo->IsVisible() == false)
		UI_TargetInfo->SetVisibility(ESlateVisibility::Visible);

	// 표시될 데이터 설정
	UI_TargetInfo->SetLevelName(UKismetSystemLibrary::GetDisplayName(&InOther));
	UI_TargetInfo->SetLevelText(TEXT("LV_00"));
}

void ACPlayer::TestKeyBinding()
{
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Slot1", EKeys::One));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Slot2", EKeys::Two));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SubSkill", EKeys::RightMouseButton));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SwapToWarrior", EKeys::One, true));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SwapToDragoon", EKeys::Two, true));
}

void ACPlayer::OffTargetInfo()
{
	if (UI_TargetInfo->IsVisible() == true)// && State->IsInBattle() == false)
		UI_TargetInfo->SetVisibility(ESlateVisibility::Hidden);
}

FHitResult ACPlayer::TraceByClick()
{
	/* 마우스 커서 위치로부터 스크린 라인 트레이스  */
	FVector start;
	FVector direction;
	Controller->DeprojectMousePositionToWorld(start, direction);

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

	return hitResult;
}

// 지정 없이 타겟팅 실행
void ACPlayer::TabOnTarget()
{
	// 타겟팅 기능 강제 ON
	Target->ToggleTarget();

	// 주변 검색해서 나온 타겟을 표시
	if (Target != nullptr)
		DisplayTargetInfo(*Target->GetTargetActor());
}

// 타겟이 될 물체 클릭 이벤트
void ACPlayer::ClickOnTarget()
{
	// 히트된 액터가 없으면 종료
	if (TraceByClick().GetActor() == nullptr)
		return;

	// 있으면 캐릭터로 캐스팅하고 타겟으로 등록
	TargetActor = Cast<ACCharacterBase>(TraceByClick().GetActor());
	Target->ToggleTarget(TargetActor);
	DisplayTargetInfo(*TargetActor);
}

void ACPlayer::DoubleClickOnTarget()
{
	// 클릭된 물체가 적인지 확인후
	if (Cast<ACEnemy>(TraceByClick().GetActor()))
	{
		// 전투 상태가 아니면 전투로 들어간다
		if (State->IsInBattle() == false)
		{
			Job->PlayEquipMotion();
		}
	}
}

void ACPlayer::OnSubAction()
{
	
}

void ACPlayer::OffSubAction()
{
}
