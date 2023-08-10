#include "UI/CUI_SkillIcon.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"

UCUI_SkillIcon::UCUI_SkillIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}


UCUI_SkillIcon::UCUI_SkillIcon(const FObjectInitializer& ObjectInitializer, const FSkillData& InData)
	: Super(ObjectInitializer)
{
	Data = InData;
}

void UCUI_SkillIcon::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCUI_SkillIcon::SetSkillData(const FSkillData& InData)
{
	Data = InData;

	if (Data.Icon == nullptr) return;

}
