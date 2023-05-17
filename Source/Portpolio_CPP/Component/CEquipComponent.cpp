#include "Component/CEquipComponent.h"

#include "CStatusComponent.h"
#include "Character/CCharacterBase.h"
#include "Character/CPlayer.h"

UCEquipComponent::UCEquipComponent()
{
	
}



void UCEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACPlayer>(GetOwner());
}

bool UCEquipComponent::IsIdleMode()
{
	return Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()))->IsIdleMode();
}

void UCEquipComponent::CheckEquipType()
{
	
}