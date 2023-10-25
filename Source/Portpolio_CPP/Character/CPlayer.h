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

// 기본 입력	
protected:
	void OnJump();

	void DisplayTargetInfo(const ACCharacterBase& InOther);		// 타겟이 된 캐릭터 정보 UI에 표시
	void TestKeyBinding();

// 타겟팅 관련
public:
	void OffTargetInfo();

	FHitResult TraceByClick();

	void TabOnTarget();									// 지정 없이 타겟팅 실행
	void ClickOnTarget();								// 타겟이 될 물체 클릭 이벤트
	void DoubleClickOnTarget();							// 타겟이 될 물체 더블 클릭 이벤트
	 
	// 스킬 관련
public:
	FActiveAvailable OnActiveAvailable;
	void ActiveAvailable(int InIndex);

	void OnSubAction();
	void OffSubAction();

	// 데미지 관련
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// UI 창
	void ShowSkillBook();

private:
	ACCharacterBase* TargetActor;
	APlayerController* Controller;

	UCUI_TargetInfo* UI_TargetInfo;
	UCHUDLayout* UI_HUDLayout;
};