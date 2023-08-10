#include "UI/CUI_SkillBook.h"

#include "CUI_SkillIcon.h"
#include "Character/CPlayer.h"
#include "Component/CJobComponent.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"

UCUI_SkillBook::UCUI_SkillBook(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UCUI_SkillBook::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerPlayer = Cast<ACPlayer>(GetOwningPlayerPawn());

	for(int i = 0; i < Book_ScrollBox->GetAllChildren().Num(); i++)
	{
		Icons.AddUnique(Cast<UCUI_SkillIcon>(Book_ScrollBox->GetChildAt(i)));
	}

	RefreshSkillData();
}

void UCUI_SkillBook::RefreshSkillData()
{
	SkillDatas = OwnerPlayer->Job->GetSkillData();

	for(int i = 0; i < Icons.Num(); i++)
	{
		Icons[i]->Icon_Image->SetBrushFromTexture(SkillDatas[i].Icon);
	}
}
