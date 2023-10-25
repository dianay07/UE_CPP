#include "UI/CUI_SkillBook.h"

#include "CUI_SkillIcon.h"
#include "Character/CPlayer.h"
#include "Component/CJobComponent.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"

UCUI_SkillBook::UCUI_SkillBook(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UCUI_SkillIcon> IconClassFinder(TEXT("WidgetBlueprint'/Game/07_UI/BP_CUI_SkillIcon.BP_CUI_SkillIcon_C'"));
	if (IconClassFinder.Succeeded())
		IconClass = IconClassFinder.Class;
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

void UCUI_SkillBook::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("%d"), Book_ScrollBox->GetChildrenCount());
}

void UCUI_SkillBook::RefreshSkillData()
{
	Book_ScrollBox->ClearChildren();

	SkillDatas = OwnerPlayer->Job->GetSkillData();

	for(int32 i = 0; i < SkillDatas.Num(); i++)
	{
		UCUI_SkillIcon* icon = CreateWidget<UCUI_SkillIcon>(GetWorld(), IconClass);
		icon->Icon_Image->SetBrushFromTexture(SkillDatas[i].Icon);
		icon->Icon_SizeBox->WidthOverride = 100.0f;
		icon->Icon_SizeBox->HeightOverride = 100.f;
		icon->SkillIndex = i;
		Book_ScrollBox->AddChild(icon);
	}
}
