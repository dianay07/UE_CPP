#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CDragWidget.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCDragWidget : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUserWidget* WidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D DragOffset; 
	
};
