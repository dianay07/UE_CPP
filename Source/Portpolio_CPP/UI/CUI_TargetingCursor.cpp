#include "UI/CUI_TargetingCursor.h"

#include "Component/CStateComponent.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UCUI_TargetingCursor::NativeConstruct()
{
	Super::NativeConstruct();

	Box->WidthOverride = 120;
	Box->HeightOverride = 400;
}

void UCUI_TargetingCursor::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UCStateComponent* State = Cast<UCStateComponent>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetComponentByClass(UCStateComponent::StaticClass()));
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *State->GetName());

	if (!State->IsInBattle())
		Image->SetBrushTintColor(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)));
	else
		Image->SetBrushTintColor(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)));
}