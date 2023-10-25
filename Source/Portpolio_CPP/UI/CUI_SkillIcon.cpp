#include "UI/CUI_SkillIcon.h"

void UCUI_SkillIcon::SetSkillData(const FSkillData& InData)
{
	Data = InData;

	if (Data.Icon == nullptr) return;
}