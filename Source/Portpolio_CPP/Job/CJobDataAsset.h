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
		TSubclassOf<class ACAttachment> AttachmentClass;				// Attachment 클래스

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> EquipmentClass;					// 장착 관리 클래스

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCSkillBase> ActiveSkillClass;				// 스킬 작동 클래스

public:
	UPROPERTY(EditAnywhere)
		FDrawWeaponData DrawWeaponData;					// 장착 애니메이션

	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> AutoAttackMontages;

	UPROPERTY(EditAnywhere)
		TArray<FSkillData> SkillDatas;									// 스킬 데이터

	UPROPERTY(EditAnywhere)
		TArray<FSkillDamageData> HitDatas;										// 히트 데이터

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCSkillBase* GetActiveSkill() { return ActiveSkill; }

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
		class UCSkillBase* ActiveSkill;

#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

};
