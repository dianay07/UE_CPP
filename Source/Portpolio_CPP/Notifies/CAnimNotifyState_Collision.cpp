#include "Notifies/CAnimNotifyState_Collision.h"

#include "Component/CJobComponent.h"
#include "Item/CAttachment.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UCJobComponent* job = Cast<UCJobComponent>(MeshComp->GetOwner()->GetComponentByClass(UCJobComponent::StaticClass()));
	if (job == nullptr) return;
	if (job->GetAttachment() == nullptr) return;

	job->GetAttachment()->OnCollision();
	//UE_LOG(LogTemp, Display, TEXT("Collision ON"));
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UCJobComponent* job = Cast<UCJobComponent>(MeshComp->GetOwner()->GetComponentByClass(UCJobComponent::StaticClass()));
	if (job == nullptr) return;
	if (job->GetAttachment() == nullptr) return;

	job->GetAttachment()->OffCollision();
	//UE_LOG(LogTemp, Display, TEXT("Collision OFF"));
}
