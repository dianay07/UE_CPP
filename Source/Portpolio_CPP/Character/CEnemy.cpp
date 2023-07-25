#include "Character/CEnemy.h"

#include "Kismet/GameplayStatics.h"
#include "CPlayer.h"
#include "Component/CEquipComponent.h"
#include "Component/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Component/CJobComponent.h"
#include "Components/BillboardComponent.h"
#include "Job/CJobStructure.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/CUI_TargetingCursor.h"

ACEnemy::ACEnemy()
{
	// CharacterBase 세팅
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh>
			SK_Mesh(TEXT("SkeletalMesh'/Game/99_Assets/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
		if(SK_Mesh.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(SK_Mesh.Object);
		}
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

		TSubclassOf<UAnimInstance> instance;
		instance = ConstructorHelpers::FClassFinder<UAnimInstance>(TEXT("AnimBlueprint'/Game/02_Enemy/ABP_Enemy.ABP_Enemy_C'")).Class;
		GetMesh()->SetAnimClass(instance);
	}

	// Component
	{
		Equip = CreateDefaultSubobject<UCEquipComponent>(TEXT("Equip Component"));
		Job = CreateDefaultSubobject<UCJobComponent>(TEXT("Job Component"));
	}

	// Billboard
	{
		CursorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("CursorBillboard"));

		static UTexture2D* CursorTexture2D = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/99_Assets/UI/Target_Battle.Target_Battle'")).Object;
		CursorBillboard->SetSprite(CursorTexture2D);
		CursorBillboard->SetupAttachment(GetMesh());
		CursorBillboard->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
		CursorBillboard->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
		CursorBillboard->SetHiddenInGame(false);
		CursorBillboard->SetVisibility(false);
	}

	// Cursor Widget 생성 및 설정
	/*CursorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Targeting Cursor"));
	CursorWidget->SetRelativeLocation(FVector(0, 0, 180.0f));
	CursorWidget->SetupAttachment(GetMesh());
	CursorWidget->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UCUI_TargetingCursor> UI_TargetingCursor(TEXT("WidgetBlueprint'/Game/07_UI/BP_CUI_TargetingCursor.BP_CUI_TargetingCursor_C'"));
	if (UI_TargetingCursor.Succeeded())
	{
		CursorWidget->SetWidgetClass(UI_TargetingCursor.Class);
		CursorWidget->SetDrawSize(FVector2D(80, 160));
		CursorWidget->SetPivot(FVector2D(0.5f, 0.8f));
		CursorWidget->SetRelativeLocation(FVector(0, 0, 400));
	}
	CursorWidget->SetVisibility(false);*/
}
 
void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Job->ChangeJob(EJob::Warrior);
}

void ACEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Hitted: Hitted(); break;
		case EStateType::Dead: Dead(); break;
	}
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FSkillDamageEvent*)&DamageEvent;

	State->SetHittedMode();

	return damage;
}

void ACEnemy::Hitted()
{
	//Apply Damage
	{
		Status->Damage(Damage.Power);
		Damage.Power = 0;
	}

	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		FSkillDamageData* data = Damage.Event->HitData;

		//data->PlayMontage(this);
		//data->PlayHitStop(GetWorld());
		//data->PlaySoundWave(this);
		data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

		if (Status->GetHealth() > 0)
		{
			FVector start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();

			//LaunchCharacter(-direction * data->Launch, false, false);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		}
	}

	if (Status->GetHealth() <= 0)
	{
		State->SetDeadMode();

		return;
	}

	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;

	State->SetIdleMode();
}

void ACEnemy::Dead()
{
}

void ACEnemy::End_Hitted()
{
	State->SetIdleMode();
}

void ACEnemy::End_Dead()
{
	Destroy();
}
