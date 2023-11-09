#include "BehaviorTree/CBTTaskNode_SkillAttack.h"

#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"
#include "Component/CAIBehaviorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AddOns/CAttackIndicator.h"

UCBTTaskNode_SkillAttack::UCBTTaskNode_SkillAttack()
{
	NodeName = "SkillAttack";
	WaitTime = 5.0f;

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_SkillAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FBTAttackTaskMemory* MyMemory = (FBTAttackTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));

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

void UCBTTaskNode_SkillAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
