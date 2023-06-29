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
		TSubclassOf<class ACAttachment> AttachmentClass;				// Attachment Ŭ����

	UPROPERTY(EditAnywhere)
		FEquipData EquipData;											// ���� �ִϸ��̼�

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> EquipmentClass;					// ���� ���� Ŭ����

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCActiveSkill> ActiveSkillClass;				// ��ų �۵� Ŭ����

	UPROPERTY(EditAnywhere)
		TArray<FSkillData> SkillDatas;									// ��ų ������

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas;										// ��Ʈ ������

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCActiveSkill* GetActiveSkill() { return ActiveSkill; }

public:
	UCJobDataAsset();

	void BeginPlay(class ACCharacterBase* InOwner);

public:
	int32 GetSkillDataCount();

public:
	UPROPERTY(EditAnywhere)
		EJob JobName;

private:
	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCActiveSkill* ActiveSkill;

#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

};
