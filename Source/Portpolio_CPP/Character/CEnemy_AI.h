#pragma once

#include "CoreMinimal.h"
#include "Character/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class PORTPOLIO_CPP_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 2;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Label")
		float LabelViewAmount = 3000.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		class ACPatrolPath* PatrolPath;
//
//#if WITH_EDITOR
//private:
//	UPROPERTY(VisibleDefaultsOnly)
//		class UWidgetComponent* LabelWidget;
//#endif

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCAIBehaviorComponent* Behavior;

public:
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }

public:
	ACEnemy_AI();

public:
	virtual void BeginPlay() override;

protected:
	void Hitted() override;
	void End_Hitted() override;

};
