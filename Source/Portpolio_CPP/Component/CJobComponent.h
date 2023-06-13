#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Job/CJobStructure.h"
#include "CJobComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJobChanged, EJob, InPrevJob, EJob, InNewJob);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCJobComponent : public UActorComponent
{
	GENERATED_BODY()

	// TODO JobComponent���� ������ �ش��ϴ� ��ų���� ������ ����
	// �����Կ� ��ų�� ��ƾ��ϴϱ�? �� ��ų�� �����ϴ� ����� ������ �ִ°� ������

public:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCJobDataAsset* DataAssets[(int32)EJob::Max];

public:	
	UCJobComponent();

protected:
	virtual void BeginPlay() override;

private:
	bool IsIdleMode();

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCActiveSkill* GetActiveSkill();

public:
	EJob GetCurrentJob();

public:
	void SetWarrior();
	void SetDragoon();

	void ChangeJob(EJob InJobType);
	void ChangeType(EJob InJobType);

	void SkillActivate1();
	
public:
	FJobChanged OnJobChanged;

public:
	class ACCharacterBase* OwnerCharacter;
	class UCStateComponent* State;

	EJob JobName;
};
