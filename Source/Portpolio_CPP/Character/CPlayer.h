#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Character/CCharacterBase.h"
#include "ICharacter.h"
#include "CPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActiveAvailable, int, InIndex);

UCLASS()
class PORTPOLIO_CPP_API ACPlayer
	: public ACCharacterBase
	, public IICharacter
	, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ACPlayer();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 1;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UCUI_TargetInfo> UI_TargetInfoClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UCHUDLayout> UI_HUDLayoutClass;

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
	FORCEINLINE USpringArmComponent* GetSpringArm() { return SpringArm; }
	FORCEINLINE ACCharacterBase* GetTargetActor() { return TargetActor; }

	FORCEINLINE UCHUDLayout* GetLayout() { return UI_HUDLayout; }
	FORCEINLINE UCJobComponent* GetJob() { return Job; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

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
	 
	// ��ų ����
public:
	FActiveAvailable OnActiveAvailable;
	void ActiveAvailable(int InIndex);

	void OnSubAction();
	void OffSubAction();

	// ������ ����
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// UI â
	void ShowSkillBook();

private:
	ACCharacterBase* TargetActor;
	APlayerController* Controller;

	UCUI_TargetInfo* UI_TargetInfo;
	UCHUDLayout* UI_HUDLayout;
};