#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Status")
		float Health;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		float MaxHealth = 200;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		float Mana;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		float MaxMana;

///////////////////////////////////////////////////// 

	UPROPERTY(VisibleAnywhere, Category = "Basic Status")
		float Strength;		// ��

	UPROPERTY(VisibleAnywhere, Category = "Basic Status")
		float Dexterity;	// ��ø

	UPROPERTY(VisibleAnywhere, Category = "Basic Status")
		float Constitution;	// Ȱ��

	UPROPERTY(VisibleAnywhere, Category = "Basic Status")
		float Intelligence;	// ����

	UPROPERTY(VisibleAnywhere, Category = "Basic Status")
		float Mind;			// ���ŷ�

	// ����
	UPROPERTY(VisibleAnywhere, Category = "Attack Status")
		float CriticalHitRate;	// �ش�ȭ

	UPROPERTY(VisibleAnywhere, Category = "Attack Status")
		float Determination;	// ������

	UPROPERTY(VisibleAnywhere, Category = "Attack Status")
		float DirectHitRate;	// ����

	// ���
	UPROPERTY(VisibleAnywhere, Category = "Depense Status")
		float Defense;			// ��������

	UPROPERTY(VisibleAnywhere, Category = "Depense Status")
		float MagicDefense;		// ��������

	// ����
	UPROPERTY(VisibleAnywhere, Category = "Melee")
		float AttackPower;		// �������ݷ�

	UPROPERTY(VisibleAnywhere, Category = "Depense Status")
		float SkillSpeed;		// ��������ӵ�

	// ����
	UPROPERTY(VisibleAnywhere, Category = "Magic Status")
		float AttackMagicPotency;	// ���ݸ��� ����

	UPROPERTY(VisibleAnywhere, Category = "Magic Status")
		float HealingMagicPotency;	// ȸ������ ����

	UPROPERTY(VisibleAnywhere, Category = "Magic Status")
		float SpellSpeed;			// �������� �ӵ�

	// ���
	UPROPERTY(VisibleAnywhere, Category = "Equip")
		float ItemLevelAverage;		// ��վ����� ����

	// ����
	UPROPERTY(VisibleAnywhere, Category = "Roll")
		float Tenacity;				// �ұ�

	UPROPERTY(VisibleAnywhere, Category = "Roll")
		float Piety;				// �ž�

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetMana() { return Mana; }
	FORCEINLINE float GetMaxMana() { return MaxMana; }

	FORCEINLINE float GetStrength() { return Strength; }
	FORCEINLINE float GetDexterity() { return Dexterity; }
	FORCEINLINE float GetConstitution() { return Constitution; }
	FORCEINLINE float GetIntelligence() { return Intelligence; }
	FORCEINLINE float GetMind() { return Mind; }

	FORCEINLINE float GetCriticalHitRate() { return CriticalHitRate; }
	FORCEINLINE float GetDetermination() { return Determination; }
	FORCEINLINE float GetDirectHitRate() { return DirectHitRate; }

	FORCEINLINE float GetDefense() { return Defense; }
	FORCEINLINE float GetMagicDefense() { return MagicDefense; }

	FORCEINLINE float GetAttackPower() { return AttackPower; }
	FORCEINLINE float GetSkillSpeed() { return SkillSpeed; }

	FORCEINLINE float GetAttackMagicPotency() { return AttackMagicPotency; }
	FORCEINLINE float GetHealingMagicPotency() { return HealingMagicPotency; }
	FORCEINLINE float GetSpellSpeed() { return SpellSpeed; }

	FORCEINLINE float GetItemLevelAverage() { return ItemLevelAverage; }

	FORCEINLINE float GetTenacity() { return Tenacity; }
	FORCEINLINE float GetPiety() { return Piety; }

public:
	//void AddEquipmentAdditionalStatus(class ACEquipment InEquipped);

public:
	void SetHealth(float InValue);
	void SetMaxHealth(float InValue);
	void SetMana(float InValue);
	void SetMaxMana(float InValue);

	void SetStrength(float InValue);
	void SetDexterity(float InValue);
	void SetConstitution(float InValue);
	void SetIntelligence(float InValue);
	void SetMind(float InValue);

	void SetCriticalHitRate(float InValue);
	void SetDetermination(float InValue);
	void SetDirectHitRate(float InValue);

	void SetDefense(float InValue);
	void SetMagicDefense(float InValue);

	void SetAttackPower(float InValue);
	void SetSkillSpeed(float InValue);

	void SetAttackMagicPotency(float InValue);
	void SetHealingMagicPotency(float InValue);
	void SetSpellSpeed(float InValue);

	void SetitemLevelAverage(float InValue);
	
	void SetTenacity(float InValue);
	void SetPiety(float InValue);

public:
	void Damage(float InAmount);
};
