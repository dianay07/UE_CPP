#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Job/CJobStructure.h"
#include "CSkillBase.generated.h"

UCLASS(Abstract)
class PORTPOLIO_CPP_API UCSkillBase : public UObject
{
	GENERATED_BODY()

public:
	UCSkillBase();

	virtual void BeginPlay
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACCharacterBase* InOwner,
		const TArray<FSkillData>& InSkillDatas,
		const TArray<FSkillDamageData>& InHitDatas
	);

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	virtual void ActiveSkill(int InIndex);
	virtual void Begin_ActiveSkill();
	virtual void End_ActiveSkill();

	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);

protected:
	bool bBeginAction;

	class ACCharacterBase* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FSkillData> SkillDatas;
	TArray<FSkillDamageData> HitDatas;
};
