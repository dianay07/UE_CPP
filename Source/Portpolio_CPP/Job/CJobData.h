#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CJobData.generated.h"


UCLASS()
class PORTPOLIO_CPP_API UCJobData : public UObject
{
	GENERATED_BODY()

private:
	friend class UJobAsset;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCSkillBase* GetActiveSkill() { return ActiveSkill; }
	FORCEINLINE class UCActiveSkill_NonGlobal* GetNonGlobal() { return NonGlobalSkill; }

public:
	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCSkillBase* ActiveSkill;

	UPROPERTY()
		class UCActiveSkill_NonGlobal* NonGlobalSkill;
};
