#include "Character/CEnemy_AI.h"

#include "Component/CAIBehaviorComponent.h"
#include "Component/CJobComponent.h"

ACEnemy_AI::ACEnemy_AI()
{
	Behavior = CreateDefaultSubobject<UCAIBehaviorComponent>(TEXT("Behavior"));
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

	Job->SetWarrior();
}

void ACEnemy_AI::Hitted()
{
	Super::Hitted();

	if (State->IsDeadMode() == true) return;

	Behavior->SetHittedMode();
}

void ACEnemy_AI::End_Hitted()
{
	Super::End_Hitted();

	Behavior->SetWaitMode();
}
