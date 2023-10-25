#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

USTRUCT()
struct FStatusList
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere, Category = "Status")
		float Health;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		float MaxHealth = 200;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		float Mana;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		float MaxMana = 200;

	///////////////////////////////////////////////////// 

	UPROPERTY(VisibleAnywhere, Category = "Basic Status")
		float Strength;		// 힘

	UPROPERTY(VisibleAnywhere, Category = "Basic Status")
		float Dexterity;	// 민첩

	UPROPERTY(VisibleAnywhere, Category = "Basic Status")
		float Constitution;	// 활력

	UPROPERTY(VisibleAnywhere, Category = "Basic Status")
		float Intelligence;	// 지능

	UPROPERTY(VisibleAnywhere, Category = "Basic Status")
		float Mind;			// 정신력

	// 공격
	UPROPERTY(VisibleAnywhere, Category = "Attack Status")
		float CriticalHitRate;	// 극대화

	UPROPERTY(VisibleAnywhere, Category = "Attack Status")
		float Determination;	// 의지력

	UPROPERTY(VisibleAnywhere, Category = "Attack Status")
		float DirectHitRate;	// 직격

	// 방어
	UPROPERTY(VisibleAnywhere, Category = "Depense Status")
		float Defense;			// 물리방어력

	UPROPERTY(VisibleAnywhere, Category = "Depense Status")
		float MagicDefense;		// 마법방어력

	// 물리
	UPROPERTY(VisibleAnywhere, Category = "Melee")
		float AttackPower;		// 물리공격력

	UPROPERTY(VisibleAnywhere, Category = "Depense Status")
		float SkillSpeed;		// 기술시전속도

	// 마법
	UPROPERTY(VisibleAnywhere, Category = "Magic Status")
		float AttackMagicPotency;	// 공격마법 위력

	UPROPERTY(VisibleAnywhere, Category = "Magic Status")
		float HealingMagicPotency;	// 회복마법 위력

	UPROPERTY(VisibleAnywhere, Category = "Magic Status")
		float SpellSpeed;			// 마법시전 속도

	// 장비
	UPROPERTY(VisibleAnywhere, Category = "Equip")
		float ItemLevelAverage;		// 평균아이템 레벨

	// 역할
	UPROPERTY(VisibleAnywhere, Category = "Roll")
		float Tenacity;				// 불굴

	UPROPERTY(VisibleAnywhere, Category = "Roll")
		float Piety;				// 신앙
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
		FStatusList Status;

public:	
	UCStatusComponent();
	UCStatusComponent(FStatusList& InStatus);

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetHealth() { return Status.Health; }
	FORCEINLINE float GetMaxHealth() { return Status.MaxHealth; }
	FORCEINLINE float GetMana() { return Status.Mana; }
	FORCEINLINE float GetMaxMana() { return Status.MaxMana; }

	FORCEINLINE float GetStrength() { return Status.Strength; }
	FORCEINLINE float GetDexterity() { return Status.Dexterity; }
	FORCEINLINE float GetConstitution() { return Status.Constitution; }
	FORCEINLINE float GetIntelligence() { return Status.Intelligence; }
	FORCEINLINE float GetMind() { return Status.Mind; }

	FORCEINLINE float GetCriticalHitRate() { return Status.CriticalHitRate; }
	FORCEINLINE float GetDetermination() { return Status.Determination; }
	FORCEINLINE float GetDirectHitRate() { return Status.DirectHitRate; }

	FORCEINLINE float GetDefense() { return Status.Defense; }
	FORCEINLINE float GetMagicDefense() { return Status.MagicDefense; }

	FORCEINLINE float GetAttackPower() { return Status.AttackPower; }
	FORCEINLINE float GetSkillSpeed() { return Status.SkillSpeed; }

	FORCEINLINE float GetAttackMagicPotency() { return Status.AttackMagicPotency; }
	FORCEINLINE float GetHealingMagicPotency() { return Status.HealingMagicPotency; }
	FORCEINLINE float GetSpellSpeed() { return Status.SpellSpeed; }

	FORCEINLINE float GetItemLevelAverage() { return Status.ItemLevelAverage; }

	FORCEINLINE float GetTenacity() { return Status.Tenacity; }
	FORCEINLINE float GetPiety() { return Status.Piety; }

public:
	//void AddEquipmentAdditionalStatus(class ACEquipment InEquipped);

public:
	UFUNCTION()
		float CalcHpPercent();

	UFUNCTION()
		FText CalcHpText();

	UFUNCTION()
		float CalcMpPercent();

	UFUNCTION()
		FText CalcMpText();

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
