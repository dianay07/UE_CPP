#include "Notifies/CAnimNotify_EndAction.h"

#include "Component/CJobComponent.h"
#include "Component/CStateComponent.h"
#include "Component/CTargetComponent.h"
#include "Item/CSkillBase.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "End_State";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UCJobComponent* job = Cast<UCJobComponent>(MeshComp->GetOwner()->GetComponentByClass(UCJobComponent::StaticClass()));
	if (job == nullptr)
		return;

	if (job->GetActiveSkill() == nullptr)
		return;

	job->GetActiveSkill()->End_ActiveSkill();

	// 액션 끝낫을때 주변에 적이 없으면 Battle상태 해제
	UCTargetComponent* Target = Cast<UCTargetComponent>(MeshComp->GetOwner()->GetComponentByClass(UCTargetComponent::StaticClass()));
	UCStateComponent* State = Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));

	if (Target->IsTargetsArrayEmpty())
	{
		State->SetIsBattle(false);
	}
}
