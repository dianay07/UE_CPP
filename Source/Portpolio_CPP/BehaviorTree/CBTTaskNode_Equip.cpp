#include "BehaviorTree/CBTTaskNode_Equip.h"

#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"
#include "Component/CJobComponent.h"
#include "Item/CEquipment.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCJobComponent* job = Cast<UCJobComponent>(ai->GetComponentByClass(UCJobComponent::StaticClass()));
	if (job == nullptr)
		return EBTNodeResult::Failed;

	//if (Type == job->JobName)
	//	return EBTNodeResult::Succeeded;

	/*switch (Type)
	{
		case EJob::Warrior:
			job->SetWarrior();
		break;

		case EJob::Dragoon: 
			job->SetDragoon();
		break;
	}*/

	job->PlayEquipMotion();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCJobComponent* job = Cast<UCJobComponent>(ai->GetComponentByClass(UCJobComponent::StaticClass()));
	const bool* bEquipped = job->GetEquipment()->GetEquipped();

	UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));
	if (state->IsIdleMode() && *bEquipped)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCJobComponent* job = Cast<UCJobComponent>(ai->GetComponentByClass(UCJobComponent::StaticClass()));
	if (job == nullptr)
		return EBTNodeResult::Failed;


	bool bBeginEquip = job->GetEquipment()->GetBeginEquip();
	if (bBeginEquip == false)
		job->GetEquipment()->Begin_Equip();

	job->GetEquipment()->End_Equip();

	return EBTNodeResult::Aborted;
}
