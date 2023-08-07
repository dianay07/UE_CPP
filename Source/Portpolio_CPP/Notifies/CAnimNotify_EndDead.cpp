#include "Notifies/CAnimNotify_EndDead.h"

#include "Character/CEnemy.h"
#include "Character/ICharacter.h"

FString UCAnimNotify_EndDead::GetNotifyName_Implementation() const
{
	return "End_Dead";
}

void UCAnimNotify_EndDead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	if (character == nullptr) return;

	ACEnemy* enemy = Cast<ACEnemy>(character);

	MeshComp->GetOwner()->Destroy();
}
