#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraControlComponent.generated.h"

USTRUCT()
struct FZoomData
{
	GENERATED_BODY()

public:
	float Speed;
	float MinRange;
	float MaxRange;
	float InterpSpeed;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCameraControlComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float HorizontalLook = 45;

	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float VerticalLook = 45;

public:
	FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

public:	
	UCameraControlComponent();

protected:
	virtual void BeginPlay() override;

private:
	class ACPlayer* OwnerCharacter;

public:
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void ApplyZoom(float InAxis);

private:
	bool bFixedCamera;
	FZoomData ZoomData;

	float Zooming;
};
