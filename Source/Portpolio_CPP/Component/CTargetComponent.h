#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTargetComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "Display")
		UDecalComponent* Decal;

public:
	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float TraceDistance = 2000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float FinishAngle = 200.0f;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float InterpSpeed = 5.0f;

public:	
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

	// �Ÿ� ������ ���� �� Ÿ���� ���
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	class ACEnemy* GetTarget();									// Ÿ�� ����

public:
	void ToggleTarget();										// Ÿ�� ���� or ����
	void Begin_Target();										// Ÿ���� �Ҷ����� �ֺ� Ÿ�� ���� ��ü�� üũ
	void End_Target();											// Ÿ���� ����
	void ChangeTarget(class ACCharacterBase* InCandidate);		// Ÿ�� ����
	void TickTargeting();										// ���� ������ �Լ������� �̻��
	bool IsTargetsArrayEmpty();

private:
	class ACCharacterBase* OwnerCharacter;
	class ACCharacterBase* Target;
	TArray<class ACCharacterBase*> Targets;

	int TargetIndex = 0;
};
