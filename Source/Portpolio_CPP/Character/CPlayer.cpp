#include "Character/CPlayer.h"

#include "CEnemy.h"
#include "Camera/CameraAnim.h"
#include "Camera/CameraComponent.h"
#include "Component/CameraControlComponent.h"
#include "Component/CEquipComponent.h"
#include "Component/CJobComponent.h"
#include "Component/CKeySettingComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CTargetComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/CSkillBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CHUDLayout.h"
#include "UI/CUI_QuickSlots.h"
#include "UI/CUI_SkillBook.h"
#include "UI/CUI_Slot.h"
#include "UI/CUI_Status.h"
#include "UI/CUI_TargetInfo.h"

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
		instance = ConstructorHelpers::FClassFinder<UAnimInstance>(
			TEXT("AnimBlueprint'/Game/01_Player/ABP_Player.ABP_Player_C'")).Class;
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
		CameraController = CreateDefaultSubobject<UCameraControlComponent>(TEXT("Camera Control"));
		Job = CreateDefaultSubobject<UCJobComponent>(TEXT("Job Component"));
		Equip = CreateDefaultSubobject<UCEquipComponent>(TEXT("Equip Component"));

		KeySettingComponent = CreateDefaultSubobject<UCKeySettingComponent>(TEXT("KeySetting Component"));
	}

	// Enemy Targeting Ui
	static ConstructorHelpers::FClassFinder<UCUI_TargetInfo> WB_InfoUI(
		TEXT("WidgetBlueprint'/Game/07_UI/BP_CUI_TargetInfo.BP_CUI_TargetInfo_C'"));
	if (WB_InfoUI.Succeeded())
	{
		UI_TargetInfoClass = WB_InfoUI.Class;
	}

	// HUDLayout
	static ConstructorHelpers::FClassFinder<UCHUDLayout> WB_HUDLayout(
		TEXT("WidgetBlueprint'/Game/07_UI/BP_CHUDLayout.BP_CHUDLayout_C'"));
	if (WB_HUDLayout.Succeeded())
	{
		UI_HUDLayoutClass = WB_HUDLayout.Class;
	}
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

	//TestKeyBinding();
	KeySettingComponent->SetBasicSetting();
	//KeySettingComponent->ChangeActionKeySetting("Slot1", EKeys::T);
	Job->ChangeJob(EJob::Warrior);
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
		//PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
		//PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACPlayer::OnJump);
		PlayerInputComponent->BindAction("CameraControl", IE_Pressed, Movement,
		                                 &UCMovementComponent::FixedCameraSetting);
		PlayerInputComponent->BindAction("CameraControl", IE_Released, Movement,
		                                 &UCMovementComponent::FixedCharacterSetting);

		PlayerInputComponent->BindAction("Targeting_Tab", IE_Pressed, this, &ACPlayer::TabOnTarget);
		PlayerInputComponent->BindAction("Targeting_Click", IE_Pressed, this, &ACPlayer::ClickOnTarget);
		PlayerInputComponent->BindAction("Targeting_Click", IE_DoubleClick, this, &ACPlayer::DoubleClickOnTarget);

		// ����Ű Ű�� ����
		PlayerInputComponent->BindAction<TDelegate<void
			(int)>>("Slot1", IE_Pressed, this, &ACPlayer::ActiveAvailable, 0);
		PlayerInputComponent->BindAction<TDelegate<void
			(int)>>("Slot2", IE_Pressed, this, &ACPlayer::ActiveAvailable, 1);
		PlayerInputComponent->BindAction<TDelegate<void
			(int)>>("Slot3", IE_Pressed, this, &ACPlayer::ActiveAvailable, 2);
		PlayerInputComponent->BindAction<TDelegate<void
			(int)>>("Slot4", IE_Pressed, this, &ACPlayer::ActiveAvailable, 3);
		PlayerInputComponent->BindAction<TDelegate<void
			(int)>>("Slot5", IE_Pressed, this, &ACPlayer::ActiveAvailable, 4);
		PlayerInputComponent->BindAction<TDelegate<void
			(int)>>("Slot6", IE_Pressed, this, &ACPlayer::ActiveAvailable, 5);
		PlayerInputComponent->BindAction<TDelegate<void
			(int)>>("Slot7", IE_Pressed, this, &ACPlayer::ActiveAvailable, 6);
		PlayerInputComponent->BindAction<TDelegate<void
			(int)>>("Slot8", IE_Pressed, this, &ACPlayer::ActiveAvailable, 7);

		//PlayerInputComponent->BindAction("SubSkill", EInputEvent::IE_Pressed, this, &ACPlayer::OnSubAction);
		//PlayerInputComponent->BindAction("SubSkill", EInputEvent::IE_Released, this, &ACPlayer::OffSubAction);

		PlayerInputComponent->BindAction("OpenBook", IE_Pressed, this, &ACPlayer::ShowSkillBook);

		// Ű �׽�Ʈ
		PlayerInputComponent->BindAction("SwapToWarrior", IE_Pressed, Job, &UCJobComponent::SetWarrior);
		PlayerInputComponent->BindAction("SwapToDragoon", IE_Pressed, Job, &UCJobComponent::SetDragoon);
	}
}

void ACPlayer::OnJump()
{
	Jump();
}

void ACPlayer::DisplayTargetInfo(const ACCharacterBase& InOther)
{
	// UI�� ������ ������ ���ְ�
	if (UI_TargetInfo->IsVisible() == false)
	{
		UI_TargetInfo->SetVisibility(ESlateVisibility::Visible);
	}

	// ǥ�õ� ������ ����
	UI_TargetInfo->SetLevelName(UKismetSystemLibrary::GetDisplayName(&InOther));
	UI_TargetInfo->SetLevelText(TEXT("LV_00"));
}

void ACPlayer::OffTargetInfo()
{
	if (UI_TargetInfo->IsVisible() == true) // && State->IsInBattle() == false)
	{
		UI_TargetInfo->SetVisibility(ESlateVisibility::Hidden);
	}
}

FHitResult ACPlayer::TraceByClick()
{
	/* ���콺 Ŀ�� ��ġ�κ��� ��ũ�� ���� Ʈ���̽�  */
	FVector start;
	FVector direction;
	Controller->DeprojectMousePositionToWorld(start, direction);

	float distance = 10000.0f;
	FVector end = start + direction * distance;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ��Ʈ ������ ������Ʈ ���� ���� �迭
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECC_Pawn);
	ObjectTypes.Add(Pawn);

	TArray<AActor*> ignores;
	ignores.AddUnique(this);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, end, ObjectTypes, false,
	                                                ignores, EDrawDebugTrace::ForDuration, hitResult, true);

	return hitResult;
}

// ���� ���� Ÿ���� ����
void ACPlayer::TabOnTarget()
{
	// Ÿ���� ��� ���� ON
	Target->ToggleTarget();

	// �ֺ� �˻��ؼ� ���� Ÿ���� ǥ��
	if (Target != nullptr)
	{
		DisplayTargetInfo(*Target->GetTargetActor());
	}
}

// Ÿ���� �� ��ü Ŭ�� �̺�Ʈ
void ACPlayer::ClickOnTarget()
{
	// ��Ʈ�� ���Ͱ� ������ ����
	if (TraceByClick().GetActor() == nullptr)
	{
		return;
	}

	// ������ ĳ���ͷ� ĳ�����ϰ� Ÿ������ ���
	TargetActor = Cast<ACCharacterBase>(TraceByClick().GetActor());
	Target->ToggleTarget(TargetActor);
	DisplayTargetInfo(*TargetActor);
}

void ACPlayer::DoubleClickOnTarget()
{
	// Ŭ���� ��ü�� ������ Ȯ����
	if (Cast<ACEnemy>(TraceByClick().GetActor()))
	{
		// ���� ���°� �ƴϸ� ������ ����
		if (State->IsInBattle() == false)
		{
			Job->PlayEquipMotion();
		}
	}
}

void ACPlayer::ActiveAvailable(int InIndex)
{
	UCUI_Slot* slot = Cast<UCUI_Slot>(UI_HUDLayout->QuickSlots->Slots->GetChildAt(InIndex));

	if (slot->AvailableType == EAvailableType::Skill && slot->GetCanUse() == true
		&& !State->IsActionMode())
	{
		GetJob()->GetSkillBase()->ActiveAvailable(slot->ItemIndex);
		slot->SetCanUse(false);
	}
}

void ACPlayer::OnSubAction()
{
	if (State->IsInBattle() == true)
	{
		Job->UseNonGlobal_Pressed();
	}
}

void ACPlayer::OffSubAction()
{
	if (State->IsInBattle() == true)
	{
		Job->UseNonGlobal_Released();
	}
}

float ACPlayer::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator,
                           AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		UE_LOG(LogActor, Warning, TEXT("%s is Take %f Radial Damage"), *this->GetName(), damage);
	}
	else
		UE_LOG(LogActor, Warning, TEXT("%s is Take %f Damage"), *this->GetName(), damage);

	GetStatus()->SetHealth(-damage);

	return damage;
}

void ACPlayer::ShowSkillBook()
{
	if (UI_HUDLayout->SkillBookWidget->GetVisibility( ) == ESlateVisibility::Hidden)
	{
		UI_HUDLayout->SkillBookWidget->RefreshSkillData();
		UI_HUDLayout->SkillBookWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UI_HUDLayout->SkillBookWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
