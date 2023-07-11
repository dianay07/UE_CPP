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

	// �ɷ�ġ, ���� ���� ������Ʈ ���
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

// TODO ������ : ���Կ� ��ȣ�� ID(��ư ����)�� �ְ� ���� ������ �����ϰԲ� �ϸ� ����
void UCJobComponent::SkillActivate(int InCount)
{
	if (GetActiveSkill() == nullptr)
	{
		return;
	}

	State->SetIsBattle(true);

	// TODO : ���� / ����� ��ų���� �ۼ�
	GetActiveSkill()->ActiveSkill(InCount);
}


// �� ����ɶ� �ؾ��� �͵�
void UCJobComponent::ChangeJob(EJob InCurrentJob)
{
	/*if (JobName == InCurrentJob)
		return;*/

		// �ش� ������ ������ ������ ���ٸ�
	if (DataAssets[static_cast<int32>(JobName)] != nullptr)
	{
		AutoAttackMontages.Reset();

		// �� ���� ����
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

// ���� ������ �� �ڵ� ����
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
	// TODO : �� �� ��ų ���� ��Ʋ���� ������?

	// Ÿ���� ������
	if (IsValid(OwnerCharacter->GetTarget()->GetTargetActor()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *OwnerCharacter->GetTarget()->GetTarget()->GetName());
		GetWorld()->GetTimerManager().SetTimer(AutoAttackTimerHandle, this, &UCJobComponent::DoingAutoAttack, AutoAttackCoolTime, true);
	}
}

// ��ų ���
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
