#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0, Evade, Equip, Hitted, Dead, Action, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsEvadeMode() { return Type == EStateType::Evade; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }

	EStateType GetType() { return Type; }

	FORCEINLINE bool IsInBattle() { return IsBattle; }
	void SetInBattle(bool InValue);

private:
	UPROPERTY(VisibleAnywhere)
		bool IsBattle = false;

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetIdleMode();
	void SetEvadeMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	void SetIsBattle(bool InValue);

private:
	UFUNCTION()
		void ChangeType(EStateType InType);

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;
};
