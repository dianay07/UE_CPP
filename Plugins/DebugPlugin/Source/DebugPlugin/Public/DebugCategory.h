#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

class DEBUGPLUGIN_API FDebugCategory
	: public FGameplayDebuggerCategory
{
public:
	FDebugCategory();
	~FDebugCategory();

public:
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

public:
	void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

private:
	struct FCategoryData
	{
		bool bDraw = false;
		FString Name;
		FVector Location;
		FVector Forward;
		float TargetArmLength;
	};
		
	FCategoryData PlayerActorData;
	FCategoryData ForwardActorData;
	FCategoryData DebugActorData;

private:
	float TraceDistance = 500;
};
