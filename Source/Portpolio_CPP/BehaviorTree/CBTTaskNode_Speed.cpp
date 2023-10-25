#include "BehaviorTree/CBTTaskNode_Speed.h"

#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCMovementComponent* movement = Cast<UCMovementComponent>(ai->GetComponentByClass(UCMovementComponent::StaticClass()));

	movement->SetSpeed(Type);

	return EBTNodeResult::Succeeded;
}