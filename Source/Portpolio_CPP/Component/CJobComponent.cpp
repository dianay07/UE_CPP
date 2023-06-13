#include "Component/CJobComponent.h"

#include "Character/CCharacterBase.h"
#include "Item/CActiveSkill.h"
#include "Job/CJobDataAsset.h"

UCJobComponent::UCJobComponent()
{
	// TODO : �� ������Ʈ������ ���⸦ ������ư���� Ŭ���ϴ°� �ƴϰ�
	// �ɺ� �������� ���� ������ �ٲ�ϱ� �׶� ���� ��������Ʈ�� ȣ��ǰ�
	// �����Ҷ� �������� ���� �ٷ� ���� ����s
}

void UCJobComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());
	for (int i = 0; i < 2; i++)
	{
		if (DataAssets[i] != nullptr)
			DataAssets[i]->BeginPlay(OwnerCharacter);
	}

	// State ����
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));

	ChangeJob(EJob::Warrior);
}

bool UCJobComponent::IsIdleMode()
{
	return Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()))->IsIdleMode();
}

ACAttachment* UCJobComponent::GetAttachment()
{
	return DataAssets[(int32)JobName]->GetAttachment();
}

UCEquipment* UCJobComponent::GetEquipment()
{
	return DataAssets[(int32)JobName]->GetEquipment();
}

UCActiveSkill* UCJobComponent::GetActiveSkill()
{
	return DataAssets[(int32)JobName]->GetActiveSkill();
}

EJob UCJobComponent::GetCurrentJob()
{
	return JobName;
}


void UCJobComponent::SetWarrior()
{
	if (IsIdleMode() == false)
		return;

	ChangeJob(EJob::Warrior);
}

void UCJobComponent::SetDragoon()
{
	if (IsIdleMode() == false)
		return;

	ChangeJob(EJob::Dragoon);
}

void UCJobComponent::ChangeJob(EJob InJobType)
{
	if (JobName == InJobType)
	{
		return;
	}

	if (DataAssets[(int32)JobName] != nullptr)
	{
		//DataAssets[(int32)JobName]->GetEquipment()->Equip();

		ChangeType(InJobType);
	}
}

void UCJobComponent::ChangeType(EJob InJobType)
{
	EJob prevJob = JobName;
	JobName = InJobType;

	if (OnJobChanged.IsBound())
		OnJobChanged.Broadcast(prevJob, InJobType);
}

// TODO ������ : ���Կ� ��ȣ�� ID(��ư ����)�� �ְ� ���� ������ �����ϰԲ� �ϸ� ����
void UCJobComponent::SkillActivate1()
{
	if (GetActiveSkill() == nullptr)
		return;

	// TODO : ���� / ����� ��ų���� �ۼ�
	GetActiveSkill()->ActiveSkill(1);
}

