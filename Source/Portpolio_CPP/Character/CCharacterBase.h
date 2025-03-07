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
	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleAnywhere)
		class UCTargetComponent* Target;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Death;

public:
	FORCEINLINE UCMovementComponent* GetMovement() { return Movement; }
	FORCEINLINE UCStatusComponent* GetStatus() { return Status; }
	FORCEINLINE UCTargetComponent* GetTarget() { return Target; }
};
