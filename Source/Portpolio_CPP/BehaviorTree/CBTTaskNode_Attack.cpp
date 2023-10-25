#include "BehaviorTree/CBTTaskNode_Attack.h"

#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"
#include "Component/CAIBehaviorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AddOns/CAttackIndicator.h"

UCBTTaskNode_Attack::UCBTTaskNode_Attack()
{
	NodeName = "Attack";
	WaitTime = 5.0f;

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FBTAttackTaskMemory* MyMemory = (FBTAttackTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	if (behavior->GetTarget() == nullptr)
		return EBTNodeResult::Failed;

	ai->PlayAnimMontage(ai->AttackMontage, 1.0f);

	if (ai->AttackIndicatorClass == nullptr)
		return EBTNodeResult::Failed;

	FTransform transform;
	transform.SetLocation(ai->GetActorLocation() + ai->GetActorForwardVector() * 300);

	ACAttackIndicator* indicator =
		GetWorld()->SpawnActorDeferred<ACAttackIndicator>(ai->AttackIndicatorClass, transform, ai, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	UGameplayStatics::FinishSpawningActor(indicator, transform);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBTAttackTaskMemory* MyMemory = (FBTAttackTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime -= DeltaSeconds;

	if (MyMemory->RemainingWaitTime <= 0.0f)
	{
		// continue execution from this node
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
