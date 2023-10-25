#include "Component/CStatusComponent.h"

UCStatusComponent::UCStatusComponent()
{
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);
}

void UCStatusComponent::SetHealth(float InValue)
{
	Health = GetHealth() + InValue;
}

void UCStatusComponent::SetMaxHealth(float InValue)
{
	MaxHealth = InValue;
}

void UCStatusComponent::SetMana(float InValue)
{
	Mana = GetMana() + InValue;
}

void UCStatusComponent::SetMaxMana(float InValue)
{
	MaxMana = InValue;
}

void UCStatusComponent::SetStrength(float InValue)
{
	Strength = GetStrength() + InValue;
}

void UCStatusComponent::SetDexterity(float InValue)
{
	Dexterity = GetDexterity() + InValue;
}

void UCStatusComponent::SetConstitution(float InValue)
{
	Constitution = GetConstitution() + InValue;
}

void UCStatusComponent::SetIntelligence(float InValue)
{
	Intelligence = GetIntelligence() + InValue;
}

void UCStatusComponent::SetMind(float InValue)
{
	Mind = GetMind() + InValue;
}

void UCStatusComponent::SetCriticalHitRate(float InValue)
{
	
}

void UCStatusComponent::SetDetermination(float InValue)
{
	
}

void UCStatusComponent::SetDirectHitRate(float InValue)
{
	
}

void UCStatusComponent::SetDefense(float InValue)
{
	
}

void UCStatusComponent::SetMagicDefense(float InValue)
{
	
}

void UCStatusComponent::SetAttackPower(float InValue)
{

}

void UCStatusComponent::SetSkillSpeed(float InValue)
{
	
}

void UCStatusComponent::SetAttackMagicPotency(float InValue)
{
	
}

void UCStatusComponent::SetHealingMagicPotency(float InValue)
{
	
}

void UCStatusComponent::SetSpellSpeed(float InValue)
{
	
}

///////////////////////////////////////////////////////////////////////

void UCStatusComponent::Damage(float InAmount)
{
	SetHealth(-1 * InAmount);
	//Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}
