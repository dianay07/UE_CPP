#include "UI/CHUDLayout.h"
#include "UI/CUI_SkillBook.h"

#include "CDragWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"


void UCHUDLayout::NativeConstruct()
{
	Super::NativeConstruct();

	SkillBookWidget = Cast<UCUI_SkillBook>(BaseCanvasPanel->GetChildAt(0));
	SkillBookWidget->SetVisibility(ESlateVisibility::Hidden);
}

bool UCHUDLayout::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                               UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UCDragWidget* DragWidgetResult = Cast<UCDragWidget>(InOperation);

	if(IsValid(DragWidgetResult) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast returned null."));
		return false;
	}

	const FVector2D DragWindowOffset = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	const FVector2D DragWindowOffsetResult = DragWindowOffset - DragWidgetResult->DragOffset;

	DragWidgetResult->WidgetRef->AddToViewport();
	DragWidgetResult->WidgetRef->SetVisibility(ESlateVisibility::Visible);
	DragWidgetResult->WidgetRef->SetPositionInViewport(DragWindowOffsetResult, false);

	return true;
}
