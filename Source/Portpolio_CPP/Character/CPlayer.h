#pragma once

#include "CoreMinimal.h"
#include "Character/CCharacterBase.h"
#include "CPlayer.generated.h"

UCLASS()
class PORTPOLIO_CPP_API ACPlayer : public ACCharacterBase
{
	GENERATED_BODY()
	
public:
	ACPlayer();

public:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
