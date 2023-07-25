#pragma once

#include "CoreMinimal.h"
#include "ICharacter.h"
#include "Character/CCharacterBase.h"
#include "CEnemy.generated.h"

UCLASS()
class PORTPOLIO_CPP_API ACEnemy
	: public ACCharacterBase
	, public IICharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		class UCJobComponent* Job;

public:
	UPROPERTY(VisibleAnywhere, Category = "Widgets")
		class UBillboardComponent* CursorBillboard;

	FORCEINLINE UBillboardComponent* GetCursor() { return CursorBillboard; }

public:
	ACEnemy();

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	void Hitted();
	void Dead();

public:
	void End_Hitted() override;
	void End_Dead() override;

private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Character;
		class AActor* Causer;

		struct FSkillDamageEvent* Event;
	} Damage;

private:
	FTimerHandle RestoreColor_TimerHandle;

public:
	UPROPERTY(VisibleAnywhere)
		class UCEquipComponent* Equip;
};
