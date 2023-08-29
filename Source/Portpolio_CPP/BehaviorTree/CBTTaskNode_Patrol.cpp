#include "BehaviorTree/CBTTaskNode_Patrol.h"

#include "CPatrolPath.h"
#include "DrawDebugHelpers.h"
#include "Components/SplineComponent.h"
#include "NavigationSystem.h"
#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"
#include "Component/CAIBehaviorComponent.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	if (!!ai->GetPatrolPath())
	{
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);

		if (bDebugMode)
			DrawDebugSphere(ai->GetWorld(), moveToPoint, 10, 10, FColor::Green, true, 5);

		return EBTNodeResult::InProgress;
	}

	FVector location = ai->GetActorLocation();

	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	if(navSystem == nullptr) return EBTNodeResult::Failed;

	FNavLocation point(location);
	while (true)
	{
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point))
			break;
	}

	behavior->SetPatrolLocation(point.Location);

	if (bDebugMode)
		DrawDebugSphere(ai->GetWorld(), point.Location, 10, 10, FColor::Green, true, 5);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));


	FVector location = behavior->GetPatrolLocation();
	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceDistance, false);

	switch (result)
	{
		case EPathFollowingRequestResult::Failed:
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
		break;

		case EPathFollowingRequestResult::AlreadyAtGoal:
		{
			if (ai->GetPatrolPath())
				ai->GetPatrolPath()->UpdateIndex();

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		break;
	}
}