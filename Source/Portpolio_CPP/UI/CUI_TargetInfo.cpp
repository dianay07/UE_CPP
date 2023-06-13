#include "UI/CUI_TargetInfo.h"

#include "Components/TextBlock.h"

void UCUI_TargetInfo::NativeConstruct()
{
	Super::NativeConstruct();

	Text_Level->SetText(FText::FromString("LV_60"));
	Text_Name->SetText(FText::FromString("Enemy Name"));
}

void UCUI_TargetInfo::SetLevelText(FString Instring)
{
	Text_Level->SetText(FText::FromString(Instring));
}

void UCUI_TargetInfo::SetLevelName(FString Instring)
{
	Text_Name->SetText(FText::FromString(Instring));
}

