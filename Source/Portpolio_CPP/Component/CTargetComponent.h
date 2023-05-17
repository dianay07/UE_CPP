#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTargetComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Settings")
		UParticleSystemComponent* Particle;

public:
	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float TraceDistance = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
		UParticleSystem* ParticleAsset;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float FinishAngle = 200.0f;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float InterpSpeed = 5.0f;

public:	
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	void ToggleTarget();

	void Begin_Target();
	void End_Target();

	void ChangeTarget(class ACCharacterBase* InChandidate);
	void TickTargeting();

private:
	class ACCharacterBase* OwnerCharacter;
	class ACCharacterBase* Target;
	TArray<class ACCharacterBase*> Targets;

};
