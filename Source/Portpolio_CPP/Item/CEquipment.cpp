#include "Item/CEquipment.h"

#include "Character/CCharacterBase.h"
#include "Component/CMovementComponent.h"

void UCEquipment::BeginPlay(ACCharacterBase* InOwner, const FEquipData& InEquipData)
{
	Owner = InOwner;
	Data = InEquipData;

	Movement = Cast<UCMovementComponent>(Owner->GetComponentByClass(UCMovementComponent::StaticClass()));
	State = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
}

void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	// 상속 후 구현
}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;

	if (OnEquipmentBeginEquip.IsBound())
		OnEquipmentBeginEquip.Broadcast();
}

void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;

	if (OnEquipmentUnequip.IsBound())
		OnEquipmentUnequip.Broadcast();
}

void UCEquipment::Unequip_Implementation()
{
	bEquipped = false;

	State->IsIdleMode();
}
