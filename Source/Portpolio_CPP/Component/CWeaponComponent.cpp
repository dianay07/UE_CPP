#include "Component/CWeaponComponent.h"

#include "Character/CCharacterBase.h"

UCWeaponComponent::UCWeaponComponent()
{
	
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACCharacterBase>(GetOwner());
}
