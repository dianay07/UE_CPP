#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
	Walk = 0, Run, Sprint, Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool CanMove() { return bCanMove; }
	FORCEINLINE void Move() { bCanMove = true; }
	FORCEINLINE void Stop() { bCanMove = false; }

	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }


public:	
	UCMovementComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ESpeedType::Max] = { 200, 400, 600 };

private:
	class ACCharacterBase* OwnerCharacter;

private:
	void SetSpeed(ESpeedType InType);

public:
	FORCEINLINE bool IsFixedCamera() { return bFixedCamera; }

public:
	void OnSprint();
	void OnRun();
	void OnWalk();

	void FixedCameraSetting();
	void FixedCharacterSetting();

public:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);

private:
	bool bCanMove = true;
	bool bFixedCamera;
};
