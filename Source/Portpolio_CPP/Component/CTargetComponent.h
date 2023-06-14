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

	// 거리 밖으로 나갈 시 타겟팅 취소
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	class ACEnemy* GetTarget();									// 타겟 리턴

public:
	void ToggleTarget();										// 타겟 시작 or 변경
	void Begin_Target();										// 타겟팅 할때마다 주변 타겟 가능 객체들 체크
	void End_Target();											// 타겟팅 종료
	void ChangeTarget(class ACCharacterBase* InCandidate);		// 타겟 변경
	void TickTargeting();										// 시점 고정용 함수였으나 미사용
	bool IsTargetsArrayEmpty();

private:
	class ACCharacterBase* OwnerCharacter;
	class ACCharacterBase* Target;
	TArray<class ACCharacterBase*> Targets;

	int TargetIndex = 0;
};
