#include "UI/CUIWindow.h"

#include "CDragWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

FReply UCUIWindow::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

void UCUIWindow::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	//UCDragWidget* DragWidget = Cast<UCDragWidget>(UWidgetBlueprintLibrary::CreateDragDropOperation(UCDragWidget::StaticClass()));
	UCDragWidget* DragDropOperatrion = NewObject<class UCDragWidget>();
	this->SetVisibility(ESlateVisibility::HitTestInvisible);

	DragDropOperatrion->WidgetRef = this;
	DragDropOperatrion->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	DragDropOperatrion->DefaultDragVisual = this;
	DragDropOperatrion->Pivot = EDragPivot::MouseDown;

	OutOperation = DragDropOperatrion;
}

void UCUIWindow::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	RemoveFromParent();
}

// �������� �巡�� ���� ��� 2���� ( �ۼ��Ǿ��ִ� ��� ��� / Ŀ���� )
// �Ѵ� ���� ���콺 Ŭ���� �巡�� Ű�ν� ����, OnDragDetecting ���۽ÿ� �Լ� ����
FReply UCUIWindow::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	/*FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

	return ReplyResult.NativeReply;*/

	if(InMouseEvent.GetEffectingButton() == DragKey)// PointerEvent.IsTouchEvent() )
	{
		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();

		if(WidgetDetectingDrag)
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();

			if(SlateWidgetDetectingDrag.IsValid())
			{
				Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);

				return Reply.NativeReply;
			}
		}
	}

	return FReply::Unhandled();
}

void UCUIWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}
