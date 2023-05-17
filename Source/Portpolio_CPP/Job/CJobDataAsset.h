#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Job/CJobStructure.h"
#include "CJobDataAsset.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCJobDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
		FEquipData EquipData;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCActiveSkill> ActiveSkillClass;

	UPROPERTY(EditAnywhere)
		TArray<FSkillData> SkillDatas;

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCActiveSkill* GetActiveSkill() { return ActiveSkill; }

public:
	UCJobDataAsset();

	void BeginPlay(class ACCharacterBase* InOwner);

	UPROPERTY(EditAnywhere)
		EJob JobName;

private:
	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCActiveSkill* ActiveSkill;
};
