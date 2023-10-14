#include "UI/CUI_Status.h"

#include <string>

#include "Character/CPlayer.h"
#include "Component/CStatusComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UCUI_Status::UCUI_Status(const FObjectInitializer& ObjectInitializer)
{

}

void UCUI_Status::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerPlayer = Cast<ACPlayer>(GetOwningPlayerPawn());

	if (OwnerPlayer == nullptr)
	{
		FString::Printf(TEXT("can't find owner character"));
		return;
	}

	Status = OwnerPlayer->GetStatus();

	//ProgressBar_Hp->SetPercent(Status->GetHealth() / Status->GetMaxHealth());
	Text_Hp->TextDelegate.BindUFunction(Status, FName("CalcHpText"));
	Text_Hp->SynchronizeProperties();

	Text_Mp->TextDelegate.BindUFunction(Status, FName("CalcMpText"));
	Text_Mp->SynchronizeProperties();

	ProgressBar_Hp->PercentDelegate.BindUFunction(Status, FName("CalcHpPercent"));
	ProgressBar_Hp->SynchronizeProperties();

	ProgressBar_Mp->PercentDelegate.BindUFunction(Status, FName("CalcMpPercent"));
	ProgressBar_Mp->SynchronizeProperties();
}

void UCUI_Status::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
