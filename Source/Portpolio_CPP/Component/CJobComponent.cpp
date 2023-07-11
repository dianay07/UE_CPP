#include "Component/CJobComponent.h"

#include "CStatusComponent.h"
#include "CTargetComponent.h"
#include "Item/CEquipment.h"
#include "Character/CCharacterBase.h"
#include "Item/CSkillBase.h"
#include "Job/CJobDataAsset.h"
#include "Kismet/KismetMathLibrary.h"

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

	//ChangeJob(EJob::Warrior);
}

void UCJobComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(OwnerCharacter->GetTarget()->GetTargetActor()))
		GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimerHandle);
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

////////////////////////////////////////////////////////////////////////////

EJob UCJobComponent::GetCurrentJob()
{
	return JobName;
}

// TODO 퀵슬롯 : 슬롯에 번호나 ID(여튼 정보)를 넣고 받은 정보를 실행하게끔 하면 슬롯
void UCJobComponent::SkillActivate(int InCount)
{
	if (GetActiveSkill() == nullptr)
	{
		return;
	}

	State->SetIsBattle(true);

	// TODO : 글쿨 / 논글쿨 스킬들어가게 작성
	GetActiveSkill()->ActiveSkill(InCount);
}


// 잡 변경될때 해야할 것들
void UCJobComponent::ChangeJob(EJob InCurrentJob)
{
	/*if (JobName == InCurrentJob)
		return;*/

		// 해당 직업의 데이터 에셋이 없다면
	if (DataAssets[static_cast<int32>(JobName)] != nullptr)
	{
		AutoAttackMontages.Reset();

		// 잡 변경 절차
		EJob prevJob = JobName;
		JobName = InCurrentJob;

		static UCJobDataAsset* dataAsset = DataAssets[static_cast<int32>(JobName)];
		for (int i = 0; i < dataAsset->AutoAttackMontages.Num(); i++)
		{
			UAnimMontage* montage = dataAsset->AutoAttackMontages[i];

			AutoAttackMontages.AddUnique(montage);
		}

		DataAssets[(int32)InCurrentJob]->GetEquipment()->Equip();

		FString EnumString = StaticEnum<EJob>()->GetNameStringByValue(static_cast<int32>(JobName));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *EnumString);
	}
}

// 전투 상태일 때 자동 공격
void UCJobComponent::DoingAutoAttack()
{
	if(State->IsInBattle() == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimerHandle);
	}

	AttackMontageIndex = UKismetMathLibrary::RandomIntegerInRange(0, AutoAttackMontages.Num() - 1);
	OwnerCharacter->PlayAnimMontage(AutoAttackMontages[AttackMontageIndex], 0.8);

	//FSkillDamageEvent event;
	//event.HitData = 
	//Cast<ACharacter>(OwnerCharacter->GetTarget())->TakeDamage(10,)
}

void UCJobComponent::OnAutoAttack()
{
	// TODO : 왜 또 스킬 쓰면 배틀상태 끝나냐?

	// 타겟이 있으면
	if (IsValid(OwnerCharacter->GetTarget()->GetTargetActor()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *OwnerCharacter->GetTarget()->GetTarget()->GetName());
		GetWorld()->GetTimerManager().SetTimer(AutoAttackTimerHandle, this, &UCJobComponent::DoingAutoAttack, AutoAttackCoolTime, true);
	}
}

// 스킬 사용
void UCJobComponent::UseSkill()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(0);

	//OnAutoAttack();
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
