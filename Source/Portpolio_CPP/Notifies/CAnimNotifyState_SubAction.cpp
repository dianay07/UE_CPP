#include "Notifies/CAnimNotifyState_SubAction.h"

#include "Component/CJobComponent.h"
#include "Job/Skill/CActiveSkill_NonGlobal.h"

FString UCAnimNotifyState_SubAction::GetNotifyName_Implementation() const
{
	return "SubAction";
}

void UCAnimNotifyState_SubAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UCJobComponent* job = Cast<UCJobComponent>(MeshComp->GetOwner()->GetComponentByClass(UCJobComponent::StaticClass()));
	if (job == nullptr) return;
	if (job->GetNonGlobal() == nullptr) return;

	job->GetNonGlobal()->Begin_SubAction();
}

void UCAnimNotifyState_SubAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UCJobComponent* job = Cast<UCJobComponent>(MeshComp->GetOwner()->GetComponentByClass(UCJobComponent::StaticClass()));
	if (job == nullptr) return;
	if (job->GetNonGlobal() == nullptr) return;

	job->GetNonGlobal()->End_SubAction();
}
