#include "Character/CCharacterBase.h"
#include "Component/CMontageComponent.h"
#include "Component/CMovementComponent.h"

ACCharacterBase::ACCharacterBase()
{
	Montages = Cast<UCMontageComponent>(GetComponentByClass(UCMontageComponent::StaticClass()));
	Movement = Cast<UCMovementComponent>(GetComponentByClass(UCMovementComponent::StaticClass()));
	State = Cast<UCStateComponent>(GetComponentByClass(UCStateComponent::StaticClass()));
}

void ACCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACCharacterBase::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{

}

