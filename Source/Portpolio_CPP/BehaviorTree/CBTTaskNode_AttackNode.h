#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_AttackNode.generated.h"

struct FBTAttackTaskMemory
{
	float RemainingWaitTime;
};

UCLASS()
class PORTPOLIO_CPP_API UCBTTaskNode_AttackNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Wait, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
		float WaitTime;

	UPROPERTY(Category = Wait, EditAnywhere, meta = (UIMin = 0, ClampMin = 0))
		float RandomDeviation;

public:
	UCBTTaskNode_AttackNode();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	class ACAIController* controller;
	class ACEnemy_AI* ai;
	class UCAIBehaviorComponent* behavior;
};
