#include "Component/CStatusComponent.h"

UCStatusComponent::UCStatusComponent()
{


}


void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

float UCStatusComponent::SetStrength(float InValue)
{
	return Strength += InValue;
}

float UCStatusComponent::SetDexterity(float InValue)
{
	return Dexterity += InValue;
}

float UCStatusComponent::SetConstitution(float InValue)
{
	return Constitution += InValue;
}

float UCStatusComponent::SetIntelligence(float InValue)
{
	return Intelligence += InValue;
}

float UCStatusComponent::SetMind(float InValue)
{
	return Mind += InValue;
}

float UCStatusComponent::SetCriticalHitRate(float InValue)
{
	return 0;
}

float UCStatusComponent::SetDetermination(float InValue)
{
	return 0;
}

float UCStatusComponent::SetDirectHitRate(float InValue)
{
	return 0;
}

float UCStatusComponent::SetDefense(float InValue)
{
	return 0;
}

float UCStatusComponent::SetMagicDefense(float InValue)
{
	return 0;
}

float UCStatusComponent::SetAttackPower(float InValue)
{
	return 0;
}

float UCStatusComponent::SetSkillSpeed(float InValue)
{
	return 0;
}

float UCStatusComponent::SetAttackMagicPotency(float InValue)
{
	return 0;
}

float UCStatusComponent::SetHealingMagicPotency(float InValue)
{
	return 0;
}

float UCStatusComponent::SetSpellSpeed(float InValue)
{
	return 0;
}
