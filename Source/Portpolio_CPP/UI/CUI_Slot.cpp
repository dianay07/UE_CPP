#include "UI/CUI_Slot.h"

#include <string>

#include "CDragWidget.h"
#include "CUI_AvailableIcon.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UCUI_Slot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (isCanUse == false)
	{
		float coolTime = Data.SkillCooltime;

		if(!availableTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(availableTimerHandle, this, &UCUI_Slot::Reset, coolTime, false);
		}
		else
		{
			// 타이머 남은 시간
			float remainTime = GetWorld()->GetTimerManager().GetTimerRemaining(availableTimerHandle);

			Icon_Image->GetDynamicMaterial()->SetScalarParameterValue("percent", (Data.SkillCooltime - remainTime ) / Data.SkillCooltime);
			UE_LOG(LogInput, Display, TEXT("%f"), (Data.SkillCooltime - remainTime));

		}
	}
}

bool UCUI_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                             UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UCDragWidget* DragWidgetResult = Cast<UCDragWidget>(InOperation);

	UCUI_AvailableIcon* Ref = Cast<UCUI_AvailableIcon>(DragWidgetResult->WidgetRef);
	Data = Ref->Data;
	Icon_Image->GetDynamicMaterial()->SetTextureParameterValue("Texture", Ref->Icon_Image->GetDynamicMaterial()->K2_GetTextureParameterValue("Texture"));
	AvailableType = Ref->AvailableType;
	ItemIndex = Ref->AvailableIndex;

	UE_LOG(LogTemp, Display, TEXT("Num : %d is regist in slot"), ItemIndex);

	return true;
}

FReply UCUI_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FString str = std::to_string(ItemIndex).c_str();
	str.Append(" Skill Clicked");

	GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, str);

	return FReply::Handled();//Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
