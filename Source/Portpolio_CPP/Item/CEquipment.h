#pragma once

#include "CoreMinimal.h"
#include "Job/CJobStructure.h"
#include "UObject/NoExportTypes.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnequip);

UCLASS()
class PORTPOLIO_CPP_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginPlay(class ACCharacterBase* InOwner, const FDrawWeaponData& InEquipData);

public:
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();

public:
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentUnequip OnEquipmentUnequip;

protected:
	class ACCharacterBase* Owner;

	FDrawWeaponData Data;

protected:
	class UCMovementComponent* Movement;
	class UCStateComponent* State;

protected:
	bool bBeginEquip;
	bool bEquipped;
};
