#pragma once

/*
 *	게임에서 Job( =직업) 관련 데이터 선언 집합
 *	각 Job이 가지고 있을 장착(Drwa), 스킬 데이터, 데미지 구조체 등을 정의
 */


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CJobStructure.generated.h"

// 직업군
UENUM(BlueprintType)
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
struct FDrawWeaponData			// 장착 애니메이션
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
struct FSkillData			// 스킬 데이터
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Basic Info")
		int DemandLevel = 0;

	UPROPERTY(EditAnywhere, Category = "Basic Info")
		FText Name;

	UPROPERTY(EditAnywhere, Category = "Basic Info")
		FText Description;

	UPROPERTY(EditAnywhere, Category = "Basic Info")
		float Damage;

	UPROPERTY(EditAnywhere, Category = "Animation")
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "Animation")
		float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
		float SkillCooltime = 2.5f;

	UPROPERTY(EditAnywhere)
		bool IsRequireTarget = false;

	UPROPERTY(EditAnywhere)
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	void ActiveSkill(class ACCharacterBase* InOwner);

	void SetSkillCooltime(float InTime);

	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);

};

USTRUCT()
struct FSkillDamageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float PlayRate = 0.0f;

	//UPROPERTY(EditAnywhere)
	//	class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	//void PlaySoundWave(class ACCharacterBase* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);

};

USTRUCT()
struct FSkillDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FSkillDamageData* HitData;
};

UCLASS()
class PORTPOLIO_CPP_API UCJobStructure : public UObject
{
	GENERATED_BODY()
	
};
