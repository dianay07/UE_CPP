#pragma once

#include "CoreMinimal.h"
#include "Job/CJobStructure.h"
#include "Job/Skill/CActiveSkill_NonGlobal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CNonGlobal_Warrior.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCNonGlobal_Warrior : public UCActiveSkill_NonGlobal
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float Distance = 1000;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float Speed = 200;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FSkillData ActionData;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FSkillDamageData HitData;

private:
	/*UPROPERTY(EditAnywhere, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;*/

public:
	virtual void Pressed() override;
	virtual void Begin_SubAction_Implementation() override;
	virtual void End_SubAction_Implementation() override;
	virtual void Tick_Implementation(float InDeltaTime) override;

private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther);

private:
	bool bMoving;

	FVector Start;
	FVector End;

	TArray<class ACharacter*> Overlapped;
	TArray<class ACharacter*> Hitted;

private:
	//class ACGhostTrail* GhostTrail;
};
