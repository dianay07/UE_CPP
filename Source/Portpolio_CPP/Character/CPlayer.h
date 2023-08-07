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
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCEquipComponent* Equip;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCJobComponent* Job;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCameraControlComponent* CameraController;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UCUI_TargetInfo> UI_TargetInfoClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UCHUDLayout> UI_HUDLayoutClass;

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() { return SpringArm; }
	FORCEINLINE ACCharacterBase* GetTargetActor() { return TargetActor; }
	FORCEINLINE UCJobComponent* GetJob() { return Job; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

// �⺻ �Է�	
protected:
	void OnJump();

	void DisplayTargetInfo(const ACCharacterBase& InOther);		// Ÿ���� �� ĳ���� ���� UI�� ǥ��
	void TestKeyBinding();

// Ÿ���� ����
public:
	void OffTargetInfo();

	FHitResult TraceByClick();

	void TabOnTarget();									// ���� ���� Ÿ���� ����
	void ClickOnTarget();								// Ÿ���� �� ��ü Ŭ�� �̺�Ʈ
	void DoubleClickOnTarget();							// Ÿ���� �� ��ü ���� Ŭ�� �̺�Ʈ

	// ���꽺ų
public:
	void OnSubAction();
	void OffSubAction();

private:
	ACCharacterBase* TargetActor;
	APlayerController* Controller;

	UCUI_TargetInfo* UI_TargetInfo;
	UCHUDLayout* UI_HUDLayout;
};