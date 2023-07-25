#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTargetComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnControlWidget);

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
	FORCEINLINE class ACCharacterBase* GetTargetActor() { return Target; }		// Ÿ���� �� ĳ���� ��ȯ

public:
	void ToggleTarget();											// TabŰ�� Ÿ�� ������ ��
	void ToggleTarget(class ACCharacterBase* InTarget);				// Ÿ���� ���콺 Ŭ������ ���� ���� ��

	void Begin_Target(class ACCharacterBase* InTarget = nullptr);	// Ÿ�� ��� ����
	void FinishTargeting();												// Ÿ�� ��� ����
	void ChangeTarget(class ACCharacterBase* InCandidate);			// Ÿ�� ����

	void ControlCursor(ACCharacterBase& InTarget);					// Ÿ�ٵ� ���� Ŀ�� On/Off

	bool IsTargetsArrayEmpty();

public:
	FOnControlWidget OnControlWidget;

private:
	class ACCharacterBase* OwnerCharacter;
	class ACCharacterBase* Target;

	class UCStateComponent* State;

// Ÿ���õ� ���� ����
private:
	TArray<class ACCharacterBase*> SearchedTargetArray;		// �˻��� ���� �迭
	int TargetIndex = 0;									// �迭 �� �ε���
};