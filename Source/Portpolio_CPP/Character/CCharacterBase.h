#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "CCharacterBase.generated.h"

UCLASS()
class PORTPOLIO_CPP_API ACCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACCharacterBase();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
		class UCMontageComponent* Montages;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleAnywhere)
		class UCTargetComponent* Target;

public:
	FORCEINLINE UCMovementComponent* GetMovement() { return Movement; }
	FORCEINLINE UCStatusComponent* GetStatus() { return Status; }
	FORCEINLINE UCTargetComponent* GetTarget() { return Target; }

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

};
