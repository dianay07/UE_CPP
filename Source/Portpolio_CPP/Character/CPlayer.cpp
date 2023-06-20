#include "Character/CPlayer.h"

#include "CEnemy.h"
#include "Camera/CameraAnim.h"
#include "Camera/CameraComponent.h"
#include "Component/CameraControlComponent.h"
#include "Component/CEquipComponent.h"
#include "Component/CJobComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CTargetComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/CUI_TargetInfo.h"

#include "Kismet/GameplayStatics.h"

ACPlayer::ACPlayer()
{
	// ACCharacterBase ����
	{
		// �޽�
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

	// CPlayer ����
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
}

void ACPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �� ���ε�
	{
		PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
		PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
		PlayerInputComponent->BindAxis("VerticalLook", CameraController, &UCameraControlComponent::OnVerticalLook);
		PlayerInputComponent->BindAxis("HorizontalLook", CameraController, &UCameraControlComponent::OnHorizontalLook);
		PlayerInputComponent->BindAxis("Zoom", CameraController, &UCameraControlComponent::ApplyZoom);
	}

	// Ű ���ε�
	{
		PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
		PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::OnJump);
		PlayerInputComponent->BindAction("CameraControl", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::FixedCameraSetting);
		PlayerInputComponent->BindAction("CameraControl", EInputEvent::IE_Released, Movement, &UCMovementComponent::FixedCharacterSetting);

		PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Job, &UCJobComponent::SkillActivate1);
		PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Pressed, this, &ACPlayer::ToggleTarget);

		PlayerInputComponent->BindAction("ClickedTarget", EInputEvent::IE_Pressed, this, &ACPlayer::ClickOnTarget);
	}
}

void ACPlayer::OnJump()
{
	Jump();
}

void ACPlayer::ToggleTarget()
{
	Target->ToggleTarget(nullptr);

	if(IsValid(Cast<ACCharacterBase>(Target->GetTarget())))
	{
		if(!UI_TargetInfo->IsVisible())
		{
			UI_TargetInfo->SetVisibility(ESlateVisibility::Visible);
		}

		UI_TargetInfo->SetLevelName(Target->GetTarget()->GetName());
	}	
	else
	{
		GLog->Log(FText::FromString("Target is Null"));

		if (UI_TargetInfo->IsVisible())
		{
			UI_TargetInfo->SetVisibility(ESlateVisibility::Hidden);
			UI_TargetInfo->SetLevelName(Target->GetTarget()->GetName());
		}
	}
}

void ACPlayer::ToggleTarget(ACCharacterBase* InOther)
{
	Target->ToggleTarget(InOther);

	if (IsValid(Cast<ACCharacterBase>(Target->GetTarget())))
	{
		if (!UI_TargetInfo->IsVisible())
		{
			UI_TargetInfo->SetVisibility(ESlateVisibility::Visible);
		}

		UI_TargetInfo->SetLevelName(Target->GetTarget()->GetName());
	}
	else
	{
		GLog->Log(FText::FromString("Target is Null"));

		if (UI_TargetInfo->IsVisible())
		{
			UI_TargetInfo->SetVisibility(ESlateVisibility::Hidden);
			UI_TargetInfo->SetLevelName(Target->GetTarget()->GetName());
		}
	}
}

void ACPlayer::ClickOnTarget()
{
	FVector start;
	FVector direction;
	Controller->DeprojectMousePositionToWorld(start, direction);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *start.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *direction.ToString());

	float distance = 10000.0f;
	FVector end = start + direction * distance;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;	// ��Ʈ ������ ������Ʈ ���� ���� �迭
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
		ToggleTarget(Cast<ACCharacterBase>(hitResult.GetActor()));
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *hitResult.GetActor()->GetName());
	}
}

void ACPlayer::Damage(ACharacter* InAttacker, TArray<ACharacter*> InDamagedObjs, FHitData InHitData)
{
	if (IsValid(Target) == false)
		return;

	for (ACharacter* candi : InDamagedObjs)
	{
		ACEnemy* enemy = Cast<ACEnemy>(candi);
		UCStatusComponent* status = Cast<UCStatusComponent>(enemy->GetComponentByClass(UCStatusComponent::StaticClass()));

		status->SetHealth(100);
	}
}
