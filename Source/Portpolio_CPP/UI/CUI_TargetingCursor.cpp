#include "UI/CUI_TargetingCursor.h"

#include "Components/Image.h"

void UCUI_TargetingCursor::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Image = Cast<UImage>(GetWidgetFromName(TEXT("Cursor Image")));
}
