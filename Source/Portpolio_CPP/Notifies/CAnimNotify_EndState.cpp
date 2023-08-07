#include "Notifies/CAnimNotify_EndState.h"
#include "Character/ICharacter.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	if(character == nullptr) return;

	switch (StateType)
	{
		case EStateType::Dead: character->Dead(); break;
		case EStateType::Hitted: character->End_Hitted(); break;
	}
}
