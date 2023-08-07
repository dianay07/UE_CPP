#include "UI/CUI_SkillBook.h"

//#include "CUI_SkillIcon.h"
#include "Character/CPlayer.h"
#include "Component/CJobComponent.h"
#include "Components/UniformGridPanel.h"

UCUI_SkillBook::UCUI_SkillBook(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//OwnerPlayer = Cast<ACPlayer>(GetOwningPlayerPawn());
}

void UCUI_SkillBook::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerPlayer = Cast<ACPlayer>(GetOwningPlayerPawn());

	SkillDatas = OwnerPlayer->Job->GetSkillData();

	for (int i = 0; SkillDatas.Num(); i++)
	{
		//static UCUI_SkillIcon* SkillIcon = NewObject<UCUI_SkillIcon>();
		//SkillIcon->SetSkillData(SkillDatas[i]);

		//UniformGridPanel->AddChildToUniformGrid(SkillIcon);
	}
}
