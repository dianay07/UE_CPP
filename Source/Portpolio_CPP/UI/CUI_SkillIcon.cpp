#include "UI/CUI_SkillIcon.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"


UCUI_SkillIcon::UCUI_SkillIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCUI_SkillIcon::NativeConstruct()
{
	Super::NativeConstruct();

	IconSize->WidthOverride = 75.0f;
	IconSize->HeightOverride = 75.0f;
	//IconSize->MinDesiredWidth = 75.0f;
	//IconSize->MinDesiredHeight = 75.0f;
}

void UCUI_SkillIcon::SetSkillData(FSkillData InSkillData)
{
	IconImage->SetBrushFromTexture(InSkillData.Icon, true);
}
