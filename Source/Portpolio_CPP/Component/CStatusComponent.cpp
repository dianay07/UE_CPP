#include "Component/CStatusComponent.h"

UCStatusComponent::UCStatusComponent()
{
	
}

UCStatusComponent::UCStatusComponent(FStatusList& InStatus)
{
	Status = InStatus;
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(Status.MaxHealth);
	SetMana(Status.MaxMana);
}

float UCStatusComponent::CalcHpPercent()
{
	return GetHealth() / GetMaxHealth(); 
}

FText UCStatusComponent::CalcHpText()
{
	FString strText;
	strText = FString::Printf(TEXT("%0.f"), GetHealth());

	return FText::FromString(strText);
}

float UCStatusComponent::CalcMpPercent()
{
	return GetMana() / GetMaxMana();
}

FText UCStatusComponent::CalcMpText()
{
	FString strText;
	strText = FString::Printf(TEXT("%0.f"), GetMana());

	return FText::FromString(strText);
}

void UCStatusComponent::SetHealth(float InValue)
{
	Status.Health = GetHealth() + InValue;
}

void UCStatusComponent::SetMaxHealth(float InValue)
{
	Status.MaxHealth = InValue;
}

void UCStatusComponent::SetMana(float InValue)
{
	Status.Mana = GetMana() + InValue;
}

void UCStatusComponent::SetMaxMana(float InValue)
{
	Status.MaxMana = InValue;
}

void UCStatusComponent::SetStrength(float InValue)
{
	Status.Strength = GetStrength() + InValue;
}

void UCStatusComponent::SetDexterity(float InValue)
{
	Status.Dexterity = GetDexterity() + InValue;
}

void UCStatusComponent::SetConstitution(float InValue)
{
	Status.Constitution = GetConstitution() + InValue;
}

void UCStatusComponent::SetIntelligence(float InValue)
{
	Status.Intelligence = GetIntelligence() + InValue;
}

void UCStatusComponent::SetMind(float InValue)
{
	Status.Mind = GetMind() + InValue;
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
