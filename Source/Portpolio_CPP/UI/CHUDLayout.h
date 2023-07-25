#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHUDLayout.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCHUDLayout : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
