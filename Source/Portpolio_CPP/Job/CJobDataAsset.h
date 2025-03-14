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

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCActiveSkill_NonGlobal> NonGlobalClass;

public:
	UPROPERTY(EditAnywhere)
		FDrawWeaponData DrawWeaponData;									// 장착 애니메이션

	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> AutoAttackMontages;

	UPROPERTY(EditAnywhere)
		TArray<FSkillData> SkillDatas;											// 스킬 데이터

	UPROPERTY(EditAnywhere)
		TArray<FSkillDamageData> HitDatas;										// 히트 데이터

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCSkillBase* GetSkillBase() { return ActiveSkill; }
	FORCEINLINE class UCActiveSkill_NonGlobal* GetNonGlobal() { return NonGlobalSkill; }

public:
	UCJobDataAsset();

	void BeginPlay(class ACCharacterBase* InOwner);

public:
	int32 GetSkillDataCount();

public:
	void SpawnAttachmentWeapon(class ACCharacterBase* InOwner);
	void DestroyAttachmentWeapon();

public:
	UPROPERTY(EditAnywhere)
		EJob JobName;

	UPROPERTY(EditAnywhere)
		float AttackRange = 0.0f;

private:
	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCSkillBase* ActiveSkill;

	UPROPERTY()
		class UCActiveSkill_NonGlobal* NonGlobalSkill;

private:
	ACharacter* Owner;

#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

};
