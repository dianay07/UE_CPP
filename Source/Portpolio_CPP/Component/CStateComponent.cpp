#include "Component/CStateComponent.h"

UCStateComponent::UCStateComponent()
{
	Type = EStateType::Idle;
}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetEvadeMode()
{
	ChangeType(EStateType::Evade);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetIsBattle(bool InValue)
{
	IsBattle = InValue;
}

void UCStateComponent::ChangeType(EStateType InType)
{
	if (this == nullptr) return;

	EStateType prevType = GetType();

	Type = InType;

	//UEnum* StateEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStateType"), true);
	//UE_LOG(LogActor, Warning, TEXT("Onwer : %s, Current State : %s"), *GetOwner()->GetName(), *StateEnum->GetNameByIndex((int32)Type).ToString());

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);
}
