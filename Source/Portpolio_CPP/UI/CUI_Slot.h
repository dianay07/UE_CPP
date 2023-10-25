#pragma once

#include "CoreMinimal.h"
#include "CUIBaseClass.h"
#include "Blueprint/UserWidget.h"
#include "CUI_Slot.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_Slot : public UCUIBaseClass
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class USizeBox* Icon_SizeBox;

	UPROPERTY(meta = (BindWidget))
		class UImage* Icon_Image;

	UPROPERTY(EditAnywhere)
		FText Name;

	UPROPERTY()
		int32 SkillIndex = 999;

protected:
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:

};
