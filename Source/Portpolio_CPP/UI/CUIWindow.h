#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUIWindow.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUIWindow : public UUserWidget
{
	GENERATED_BODY()

protected:
	// �巡�� ���� ( Ŭ�� �ν� )
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);

	virtual void NativeOnInitialized() override;

};
