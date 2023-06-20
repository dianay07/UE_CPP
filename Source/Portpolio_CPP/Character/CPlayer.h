#pragma once

#include "CoreMinimal.h"
#include "Character/CCharacterBase.h"
#include "ICharacter.h"
#include "CPlayer.generated.h"

UCLASS()
class PORTPOLIO_CPP_API ACPlayer : public ACCharacterBase, public IICharacter
{
	GENERATED_BODY()
	
public:
	ACPlayer();

public:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class UCEquipComponent* Equip;

	UPROPERTY(VisibleAnywhere)
		class UCJobComponent* Job;

	UPROPERTY(VisibleAnywhere)
		class UCameraControlComponent* CameraController;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class UCUI_TargetInfo> UI_TargetInfoClass;

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() { return SpringArm; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void OnJump();

	void ToggleTarget();
	void ToggleTarget(ACCharacterBase* InOther);

	void ClickOnTarget();

	virtual void Damage(ACharacter* InAttacker, TArray<ACharacter*> InDamagedObjs, FHitData InHitData) override;

private:
	UCUI_TargetInfo* UI_TargetInfo;
	APlayerController* Controller;
};