#include "Item/CEquipment.h"

#include "Character/CCharacterBase.h"
#include "Component/CMovementComponent.h"

void UCEquipment::BeginPlay(ACCharacterBase* InOwner, const FDrawWeaponData& InEquipData)
{
	Owner = InOwner;
	Data = InEquipData;

	Movement = Cast<UCMovementComponent>(Owner->GetComponentByClass(UCMovementComponent::StaticClass()));
	State = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
}

void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	//if (Data.bCanMove == false)
	//	Movement->Stop();

	//if (Data.bUseControlRotation)
	//	Movement->EnableControlRotation();

	if (!!Data.Montage)
	{
		Owner->PlayAnimMontage(Data.Montage, Data.PlayRate);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}
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
