#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/CBTTaskNode_AttackNode.h"
#include "CBTTaskNode_SkillAttack.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCBTTaskNode_SkillAttack : public UCBTTaskNode_AttackNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_SkillAttack();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
