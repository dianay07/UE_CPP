#include "BehaviorTree/CBTTaskNode_Attack.h"

#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"
#include "Component/CAIBehaviorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

#include "Kismet/KismetMaterialLibrary.h"

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

	float Damage = 50.0f;
	UGameplayStatics::ApplyDamage(behavior->GetTarget(), Damage, ai->GetController(), nullptr, NULL);
	UE_LOG(LogActor, Warning, TEXT("%s is Attack %s, DamageAmount is %f"), *ai->GetName(), *behavior->GetTarget()->GetName(), Damage);

	ai->PlayAnimMontage(ai->AttackMontage, 1.0f);
	//Decal = GetWorld()->SpawnActor<ADecalActor>(FVector(behavior->GetTarget()->GetActorLocation().X,
		//behavior->GetTarget()->GetActorLocation().Y, -22745.0f), FRotator());
	//Decal->SetDecalMaterial(ai->DecalActorTest);
	//Decal->GetDecal()->DecalSize = FVector(15.0f, 30.0f, 30.0f);
	
	//Decal->SetLifeSpan(3.0f);


	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBTAttackTaskMemory* MyMemory = (FBTAttackTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime -= DeltaSeconds;

	if(IsValid(Decal))
	{
		FVector DecalSize = Decal->GetDecal()->DecalSize;

		//Decal->GetDecal()->DecalSize = FVector(DecalSize.X, DecalSize.Y + DeltaSeconds * 500.0f, DecalSize.Z + DeltaSeconds * 500.0f);
		//float testValue = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), test, TEXT("Radius"));
		//UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), test, TEXT("Radius"), testValue + DeltaSeconds);
	}
		

	if (MyMemory->RemainingWaitTime <= 0.0f)
	{
		// continue execution from this node
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
