#include "Notifies/CAnimNotify_EndAction.h"

#include "Component/CJobComponent.h"
#include "Item/CActiveSkill.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "End_Action";
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
}
