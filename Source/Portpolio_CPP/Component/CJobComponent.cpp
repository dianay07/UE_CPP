#include "Component/CJobComponent.h"

#include "Character/CCharacterBase.h"
#include "Item/CActiveSkill.h"
#include "Job/CJobDataAsset.h"

UCJobComponent::UCJobComponent()
{
	// TODO : 내 프로젝트에서는 무기를 장착버튼으로 클릭하는게 아니고
	// 심볼 아이템을 끼면 직업이 바뀌니까 그때 변경 델리게이트가 호출되게
	// 시작할때 아이템을 끼면 바로 직업 설정s
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

	// State 설정
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

// TODO 퀵슬롯 : 슬롯에 번호나 ID(여튼 정보)를 넣고 받은 정보를 실행하게끔 하면 슬롯
void UCJobComponent::SkillActivate1()
{
	if (GetActiveSkill() == nullptr)
		return;

	// TODO : 글쿨 / 논글쿨 스킬들어가게 작성
	GetActiveSkill()->ActiveSkill(1);
}

