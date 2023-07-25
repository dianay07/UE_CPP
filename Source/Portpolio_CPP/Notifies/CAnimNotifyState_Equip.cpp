#include "Notifies/CAnimNotifyState_Equip.h"

#include "Component/CJobComponent.h"
#include "Item/CEquipment.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	UCJobComponent* job = Cast<UCJobComponent>(MeshComp->GetOwner()->GetComponentByClass(UCJobComponent::StaticClass()));
	if(job == nullptr)
		return;

	if (job->GetEquipment() == nullptr)
		return;

	job->GetEquipment()->Begin_Equip();
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	UCJobComponent* job = Cast<UCJobComponent>(MeshComp->GetOwner()->GetComponentByClass(UCJobComponent::StaticClass()));
	if (job == nullptr)
		return;

	if (job->GetEquipment() == nullptr)
		return;

	job->GetEquipment()->End_Equip();
}
