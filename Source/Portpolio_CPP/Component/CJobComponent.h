#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Job/CJobStructure.h"
#include "CJobComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJobChanged, EJob, InPrevJob, EJob, InNewJob);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActivateSkill, int, InSkillIndex);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCJobComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCJobDataAsset* DataAssets[(int32)EJob::Max];

	UPROPERTY(EditAnywhere, Category = "Montage")
		TArray<UAnimMontage*> AutoAttackMontages;

private:
	UPROPERTY(VisibleAnywhere, Category = "Job Status")
		float AutoAttackCoolTime = 2.5f;

public:	
	UCJobComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	class ACAttachment* GetAttachment();		// 인게임에서 실장착될 액터
	class UCEquipment* GetEquipment();			// 장착 기능을 담당할 클래스
	class UCSkillBase* GetActiveSkill();		// 스킬을 실행할 클래스

	EJob GetCurrentJob();

public:
	UFUNCTION()
		void SkillActivate(int InCount);

	UFUNCTION()
		void ChangeJob(EJob InCurrentJob);

	void DoingAutoAttack();
	void OnAutoAttack();
	void UseSkill();						// 스킬 실행

	void SetWarrior();
	void SetDragoon();

private:
	bool IsIdleMode();

public:
	FJobChanged OnJobChanged;
	FActivateSkill OnSkillActivate;

public:
	class ACCharacterBase* OwnerCharacter;
	class UCStateComponent* State;
	class UCStatusComponent* Status;

	EJob JobName;
	
	// 자동 공격 관련
private:
	int32 AttackMontageIndex;
	FTimerHandle AutoAttackTimerHandle;
};
