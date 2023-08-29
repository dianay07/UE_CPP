#include "BehaviorTree/CBTService_Melee.h"

#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"
#include "Component/CAIBehaviorComponent.h"
#include "Component/CStateComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));
	UCAIBehaviorComponent* aiState = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	if (state->IsHittedMode())
	{
		aiState->SetHittedMode();

		return;
	}

	ACharacter* target = aiState->GetTarget();
	if (target == nullptr)
	{
		aiState->SetPatrolMode();

		return;
	}

	float distance = ai->GetDistanceTo(target);
	if (distance < ActionRange)
	{
		aiState->SetActionMode();

		return;
	}

	aiState->SetApproachMode();
}