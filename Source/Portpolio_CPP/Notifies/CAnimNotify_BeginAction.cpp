#include "Notifies/CAnimNotify_BeginAction.h"

#include "Component/CJobComponent.h"
#include "Item/CActiveSkill.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "Begin_Action";
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UCJobComponent* job = Cast<UCJobComponent>(MeshComp->GetOwner()->GetComponentByClass(UCJobComponent::StaticClass()));
	if (job == nullptr)
		return;

	if (job->GetActiveSkill() == nullptr)
		return;

	job->GetActiveSkill()->Begin_ActiveSkill();
}
