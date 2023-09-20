#include "UI/CUI_AvailableIcon.h"

void UCUI_AvailableIcon::SetSkillData(const FSkillData& InData)
{
	Data = InData;

	if (Data.Icon == nullptr) return;
}