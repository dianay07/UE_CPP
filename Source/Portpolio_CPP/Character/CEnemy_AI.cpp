#include "Character/CEnemy_AI.h"

#include "Component/CAIBehaviorComponent.h"

ACEnemy_AI::ACEnemy_AI()
{
	Behavior = CreateDefaultSubobject<UCAIBehaviorComponent>(TEXT("Behavior"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE(TEXT("AnimMontage'/Game/05_Montage/Enemy/Wolf/Anim_WolfAttack_Montage.Anim_WolfAttack_Montage'"));
	if(MONTAGE.Succeeded())
	{
		AttackMontage = MONTAGE.Object;
	}
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();
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
