#include "UI/CUI_Slot.h"

#include <string>

#include "CDragWidget.h"
#include "CUI_SkillIcon.h"
#include "Components/Image.h"



bool UCUI_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                             UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UCDragWidget* DragWidgetResult = Cast<UCDragWidget>(InOperation);

	UCUI_SkillIcon* RefIcon = Cast<UCUI_SkillIcon>(DragWidgetResult->WidgetRef);
	Icon_Image->SetBrush(RefIcon->Icon_Image->Brush);
	SkillIndex = RefIcon->SkillIndex;

	UE_LOG(LogTemp, Display, TEXT("Num : %d is regist in slot"), SkillIndex);

	return true;
}

FReply UCUI_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FString str = std::to_string(SkillIndex).c_str();
	str.Append(" Skill Clicked");

	GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, str);

	return FReply::Handled();//Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
