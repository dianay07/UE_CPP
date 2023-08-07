#pragma once

#include "CoreMinimal.h"
#include "Component/CJobComponent.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float Speed;

	UPROPERTY(BlueprintReadOnly)
		float Direction;

	UPROPERTY(BlueprintReadOnly)
		float Pitch;

	UPROPERTY(BlueprintReadOnly)
		bool IsJump;

	UPROPERTY(BlueprintReadOnly)
		bool IsFixedCamera;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool IsBattle;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		EJob JobType = EJob::Max;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class ACPlayer* Owner;
	UCJobComponent* Job;

private:
	FRotator PrevRotation;

	UFUNCTION()
		void OnJobTypeChanged(EJob InPrevType, EJob InNewType);

};
