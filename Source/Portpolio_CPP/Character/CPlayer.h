#pragma once

#include "CoreMinimal.h"
#include "Character/CCharacterBase.h"
#include "ICharacter.h"
#include "CPlayer.generated.h"

UCLASS()
class PORTPOLIO_CPP_API ACPlayer
	: public ACCharacterBase
	, public IICharacter
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
	FORCEINLINE ACCharacterBase* GetTargetActor() { return TargetActor; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void OnJump();

	void DisplayTargetInfo(ACCharacterBase* InOther);		// Ÿ���� �� ĳ���� ���� UI�� ǥ��
	void TestKeyBinding();

public:
	void OffTargetInfo();
	void TabOnTarget();										// ���� ���� Ÿ���� ����
	void ClickOnTarget();									// Ÿ���� �� ��ü Ŭ�� �̺�Ʈ

private:
	UCUI_TargetInfo* UI_TargetInfo;
	ACCharacterBase* TargetActor;
	APlayerController* Controller;
};