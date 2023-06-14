#include "Character/CPlayer.h"

#include "CEnemy.h"
#include "Camera/CameraAnim.h"
#include "Camera/CameraComponent.h"
#include "Component/CameraControlComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CEquipComponent.h"
#include "Component/CJobComponent.h"
#include "Component/CTargetComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/CUI_TargetInfo.h"

#include "UI/CUI_TargetingCursor.h"

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

		PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Job, &UCJobComponent::SkillActivate1);
		PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Pressed, this, &ACPlayer::ToggleTarget);
	}
}

void ACPlayer::OnJump()
{
	Jump();
}

void ACPlayer::ToggleTarget()
{
	Target->ToggleTarget();

	if(IsValid(Cast<ACCharacterBase>(Target->GetTarget())))
	{
		//Target->GetTarget()->ActiveTargetCursor();

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
