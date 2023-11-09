#include "BehaviorTree/CBTTaskNode_AttackNode.h"

#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"
#include "Component/CAIBehaviorComponent.h"
#include "AddOns/CAttackIndicator.h"

UCBTTaskNode_AttackNode::UCBTTaskNode_AttackNode()
{
	//NodeName = "SkillAttack";
	WaitTime = 5.0f;

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_AttackNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTAttackTaskMemory* MyMemory = (FBTAttackTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));

	controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ai = Cast<ACEnemy_AI>(controller->GetPawn());
	behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	if (behavior->GetTarget() == nullptr)
		return EBTNodeResult::Failed;

	FTransform transform;
	transform.SetLocation(ai->GetActorLocation() + ai->GetActorForwardVector() * 300);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_AttackNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBTAttackTaskMemory* MyMemory = (FBTAttackTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime -= DeltaSeconds;

	if (MyMemory->RemainingWaitTime <= 0.0f)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
