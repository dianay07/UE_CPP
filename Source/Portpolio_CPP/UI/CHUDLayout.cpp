#include "UI/CHUDLayout.h"
#include "UI/CUI_SkillBook.h"

#include "CDragWidget.h"
#include "CUI_QuickSlots.h"
#include "CUI_SkillIcon.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"


void UCHUDLayout::NativeConstruct()
{
	Super::NativeConstruct();

	SkillBookWidget = Cast<UCUI_SkillBook>(BaseCanvasPanel->GetChildAt(0));
	SkillBookWidget->SetVisibility(ESlateVisibility::Hidden);

	Slots = Cast<UCUI_QuickSlots>(BaseCanvasPanel->GetChildAt(1));
}

bool UCHUDLayout::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UCDragWidget* DragWidgetResult = Cast<UCDragWidget>(InOperation);

	if (IsValid(DragWidgetResult) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast returned null."));
		return false;
	}

	const FVector2D DragWindowOffset = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	const FVector2D DragWindowOffsetResult = DragWindowOffset - DragWidgetResult->DragOffset;

	if (Cast<UCUI_SkillIcon>(DragWidgetResult->WidgetRef))
		return false;

	DragWidgetResult->WidgetRef->AddToViewport();
	DragWidgetResult->WidgetRef->SetVisibility(ESlateVisibility::Visible);

	DragWidgetResult->WidgetRef->SetPositionInViewport(DragWindowOffsetResult, false);

	return true;
}
