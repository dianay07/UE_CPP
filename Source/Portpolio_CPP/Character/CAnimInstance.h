#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		float Speed;

	UPROPERTY(BlueprintReadOnly)
		float Direction;

	UPROPERTY(BlueprintReadOnly)
		float Pitch;

	UPROPERTY(BlueprintReadOnly)
		bool IsJump;

	UPROPERTY(BlueprintReadOnly)
		bool IsFixedCamera;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class ACPlayer* Owner;

private:
	FRotator PrevRotation;

};
