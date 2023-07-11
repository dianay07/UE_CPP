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

	// 거리 밖으로 나갈 시 타겟팅 취소
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE class ACCharacterBase* GetTargetActor() { return Target; }		// 타겟이 된 캐릭터 반환

public:
	void ToggleTarget();											// Tab키로 타겟 시작할 때
	void ToggleTarget(class ACCharacterBase* InTarget);				// 타겟이 마우스 클릭으로 지정 됬을 때

	void Begin_Target(class ACCharacterBase* InTarget = nullptr);	// 타겟 기능 실행
	void End_Target();												// 타겟 기능 종료
	void ChangeTarget(class ACCharacterBase* InCandidate);			// 타겟 변경

	void ControlCursor(ACCharacterBase* InTarget);					// 타겟된 액터 커서 On/Off

	void TickTargeting();											// 
	bool IsTargetsArrayEmpty();

public:
	FOnControlWidget OnControlWidget;

private:
	class ACCharacterBase* OwnerCharacter;
	class ACCharacterBase* Target;

	class UCStateComponent* State;
	TArray<class ACCharacterBase*> Targets;

	int TargetIndex = 0;
};