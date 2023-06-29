#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CJobStructure.generated.h"

// 직업군
UENUM()
enum class EJob : uint8
{
	Warrior, Dragoon, Max
};

// 스킬 쿨타임 타입
UENUM()
enum class ESkillCoolType : uint8
{
	Global, NonGlobal
};

USTRUCT()
struct FEquipData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bUseControllerRotate = true;
};

USTRUCT()
struct FSkillData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		int DemandLevel = 0;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
		bool bUseControllerRotate = false;

	UPROPERTY(EditAnywhere)
		ESkillCoolType Type = ESkillCoolType::Global;

	UPROPERTY(EditAnywhere)
		float SkillCooltime = 0.0f;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	void ActiveSkill(class ACCharacterBase* InOwner);
	void SetSkillCooltime(float InTime);
};

USTRUCT()
struct FHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
		float Damage = 0.0f;

	UPROPERTY(EditAnywhere)
		class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	void SendDamage(class ACCharacterBase* InAttacker, AActor* InAttackCauser, class ACCharacterBase* InOther);
	void PlaySoundWave(class ACCharacterBase* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);
};

USTRUCT()
struct FSkillDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FHitData* HitData;
};

UCLASS()
class PORTPOLIO_CPP_API UCJobStructure : public UObject
{
	GENERATED_BODY()
	
};
