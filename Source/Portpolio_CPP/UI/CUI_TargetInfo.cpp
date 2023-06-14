#include "UI/CUI_TargetInfo.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/CStateComponent.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UCUI_TargetInfo::UCUI_TargetInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> tex1(TEXT("/Game/99_Assets/UI/border_yellow"));
	ProgressBar_HP_Border_Wait = tex1.Object;
	Border_WaitBrush = UWidgetBlueprintLibrary::MakeBrushFromTexture(ProgressBar_HP_Border_Wait, 200, 7);

	static ConstructorHelpers::FObjectFinder<UTexture2D> tex2(TEXT("/Game/99_Assets/UI/border_red"));
	ProgressBar_Hp_Border_Battle = tex2.Object;
	Border_BattleBrush = UWidgetBlueprintLibrary::MakeBrushFromTexture(ProgressBar_Hp_Border_Battle, 200, 7);

}

void UCUI_TargetInfo::NativeConstruct()
{
	Super::NativeConstruct();

	//HPBar_Overlay->Slot->Parent

	Text_Level->SetText(FText::FromString("LV_60"));
	Text_Name->SetText(FText::FromString("Enemy Name"));
}

void UCUI_TargetInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 플레이어의 StateComponent 콜
	UCStateComponent* State = Cast<UCStateComponent>(GetOwningPlayerPawn()->GetComponentByClass(UCStateComponent::StaticClass()));
	//FString str = State->IsBattle() ? TEXT("true") : TEXT("false");
	//UE_LOG(LogTemp, Warning, L"%s", *str);

	if (State->IsInBattle() == true)
	{
		ProgressBar_HP->WidgetStyle.SetBackgroundImage(Border_BattleBrush);
		ProgressBar_HP->SetFillColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.1f));
	}
	else
	{
		ProgressBar_HP->WidgetStyle.SetBackgroundImage(Border_WaitBrush);
		ProgressBar_HP->SetFillColorAndOpacity(FLinearColor(1.0f, 0.57f, 0.11f));
	}
		
}

void UCUI_TargetInfo::SetLevelText(FString Instring)
{
	Text_Level->SetText(FText::FromString(Instring));
}

void UCUI_TargetInfo::SetLevelName(FString Instring)
{
	Text_Name->SetText(FText::FromString(Instring));
}

