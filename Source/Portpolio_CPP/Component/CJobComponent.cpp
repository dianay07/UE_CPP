#include "Component/CJobComponent.h"

#include "CStatusComponent.h"
#include "CTargetComponent.h"
#include "Utility.h"
#include "Character/CCharacterBase.h"
#include "Item/CEquipment.h"
#include "Item/CSkillBase.h"
#include "Job/CJobDataAsset.h"
#include "Job/Skill/CActiveSkill_NonGlobal.h"

UCJobComponent::UCJobComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCJobComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());
	for (int i = 0; i < 2; i++)
	{
		if (DataAssets[i] != nullptr)
		{
			DataAssets[i]->BeginPlay(OwnerCharacter);
		}
	}

	// 능력치, 상태 관리 컴포넌트 등록
	Status = Cast<UCStatusComponent>(OwnerCharacter->GetComponentByClass(UCStatusComponent::StaticClass()));
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));

	OnSkillActivate.AddDynamic(this, &UCJobComponent::SkillActivate);
}

void UCJobComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(OwnerCharacter->GetTarget()->GetTargetActor()))
		GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimerHandle);

	if (!!GetNonGlobal())
		GetNonGlobal()->Tick(DeltaTime);
}

ACAttachment* UCJobComponent::GetAttachment()
{
	return DataAssets[static_cast<int32>(JobName)]->GetAttachment();
}

UCEquipment* UCJobComponent::GetEquipment()
{
	return DataAssets[static_cast<int32>(JobName)]->GetEquipment();
}

UCSkillBase* UCJobComponent::GetActiveSkill()
{
	return DataAssets[static_cast<int32>(JobName)]->GetActiveSkill();
}

UCActiveSkill_NonGlobal* UCJobComponent::GetNonGlobal()
{
	if (State->IsInBattle() == false) return nullptr;

	return DataAssets[static_cast<int32>(JobName)]->GetNonGlobal();
}

TArray<FSkillData> UCJobComponent::GetSkillData()
{
	return DataAssets[static_cast<int32>(JobName)]->SkillDatas;
}

////////////////////////////////////////////////////////////////////////////

EJob UCJobComponent::GetCurrentJob()
{
	return JobName;
}

// TODO 퀵슬롯 : 슬롯에 번호나 ID(여튼 정보)를 넣고 받은 정보를 실행하게끔 하면 슬롯
void UCJobComponent::SkillActivate(int InCount)
{
	if (GetActiveSkill() == nullptr)
		return;

	// 공격 거리
	if (OwnerCharacter->GetTarget()->GetTargetActor() == nullptr)
		return;

	distance = UKismetMathLibrary::Vector_Distance(OwnerCharacter->GetActorLocation(), OwnerCharacter->GetTarget()->GetTargetActor()->GetActorLocation());
	if (distance > DataAssets[static_cast<int32>(JobName)]->AttackRange)
		return;

	State->SetIsBattle(true);

	// TODO : 글쿨 / 논글쿨 스킬들어가게 작성
	GetActiveSkill()->ActiveSkill(InCount);
}


// 잡 변경될때 해야할 것들
void UCJobComponent::ChangeJob(EJob InCurrentJob)
{
	if (State->IsInBattle())
		return;

	// 해당 직업의 데이터 에셋 갱신
	if (DataAssets[static_cast<int32>(JobName)] != nullptr)
	{
		DataAssets[static_cast<int32>(JobName)]->DestroyAttachmentWeapon();

		AutoAttackMontages.Reset();

		// 잡 변경 절차
		EJob prevJob = JobName;
		JobName = InCurrentJob;

		if (JobName == EJob::Max)
			return;

		static UCJobDataAsset* dataAsset = DataAssets[static_cast<int32>(JobName)];
		for (int i = 0; i < dataAsset->AutoAttackMontages.Num(); i++)
		{
			static UAnimMontage* montage = dataAsset->AutoAttackMontages[i];

			AutoAttackMontages.AddUnique(montage);
		}

		FString EnumString = StaticEnum<EJob>()->GetNameStringByValue(static_cast<int32>(JobName));
		OnJobChanged.Broadcast(prevJob, InCurrentJob);

		DataAssets[static_cast<int32>(JobName)]->SpawnAttachmentWeapon(OwnerCharacter);

		Utility::PlayEffect(OwnerCharacter->GetWorld(), ParticleAsset, 
			FTransform(OwnerCharacter->GetActorLocation() + FVector(0,0,-90)));
	}
}

void UCJobComponent::PlayEquipMotion()
{
	DataAssets[(int32)JobName]->GetEquipment()->Equip();
	State->SetIsBattle(true);
}

void UCJobComponent::PlayUnequipMotion()
{
	DataAssets[(int32)JobName]->GetEquipment()->Unequip();
	State->SetIsBattle(false);
}

// 전투 상태일 때 자동 공격
void UCJobComponent::DoingAutoAttack()
{
	if(State->IsInBattle() == false)
		GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimerHandle);

	// 공격 거리
	distance = UKismetMathLibrary::Vector_Distance(OwnerCharacter->GetActorLocation(), OwnerCharacter->GetTarget()->GetTargetActor()->GetActorLocation());
	if (distance > DataAssets[static_cast<int32>(JobName)]->AttackRange)
		return;

	AttackMontageIndex = UKismetMathLibrary::RandomIntegerInRange(0, AutoAttackMontages.Num() - 1);
	OwnerCharacter->PlayAnimMontage(AutoAttackMontages[AttackMontageIndex], 0.8);
}

void UCJobComponent::OnAutoAttack()
{
	// 타겟이 있으면
	if (IsValid(OwnerCharacter->GetTarget()->GetTargetActor()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *OwnerCharacter->GetTarget()->GetTarget()->GetName());
		GetWorld()->GetTimerManager().SetTimer(AutoAttackTimerHandle, this, &UCJobComponent::DoingAutoAttack, AutoAttackCoolTime, true);
	}
}

// 스킬 사용
void UCJobComponent::UseFirstSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(0);
}

void UCJobComponent::UseSecondSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(1);
}

void UCJobComponent::UseNonGlobal_Pressed()
{
	if (!!GetNonGlobal())
		GetNonGlobal()->Pressed();
}

void UCJobComponent::UseNonGlobal_Released()
{
	if (!!GetNonGlobal())
		GetNonGlobal()->Released();
}


///////////////////////////////////////////////////////////////////////////////////////

void UCJobComponent::SetWarrior()
{
	if (IsIdleMode() == false)
	{
		return;
	}

	ChangeJob(EJob::Warrior);
}

void UCJobComponent::SetDragoon()
{
	if (IsIdleMode() == false)
	{
		return;
	}

	ChangeJob(EJob::Dragoon);
}

bool UCJobComponent::IsIdleMode()
{
	return Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()))->IsIdleMode();
}
