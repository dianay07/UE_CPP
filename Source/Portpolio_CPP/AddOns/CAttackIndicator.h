#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttackIndicator.generated.h"

UCLASS()
class PORTPOLIO_CPP_API ACAttackIndicator : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* DefaultRoot = nullptr;

	UPROPERTY(VisibleAnywhere)
		UDecalComponent* Circum;

	UPROPERTY(VisibleAnywhere)
		UDecalComponent* Area;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", ClampMax = "0.5", ToolTip = "Boundary Min 0.1, Max 0.5"))
		float BoundarySize;

	// 기본 10, 최소 0.1배, 최대 2배
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "20", ToolTip = "Base 10, Min 0.1, Max 2"))
		float ActionSpeed = 10.0f;

public:	
	ACAttackIndicator();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Custom Function
public:
	void ApplyDamage();

private:
	UMaterial* material;

	UMaterialInstanceDynamic* CircumRef;
	UMaterialInstanceDynamic* AreaRef;
};
