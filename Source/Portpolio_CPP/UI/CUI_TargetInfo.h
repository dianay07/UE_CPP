#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_TargetInfo.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_TargetInfo : public UUserWidget
{
	GENERATED_BODY()

private:
	//UPROPERTY(VisibleAnywhere, meta=(Bindwidget))
	//	class USizeBox* InfoBox;
	UPROPERTY(VisibleAnywhere, meta = (Bindwidget))
		class UOverlay* HPBar_Overlay;

	UPROPERTY(VisibleAnywhere, meta = (Bindwidget))
		class UTextBlock* Text_Level;

	UPROPERTY(VisibleAnywhere, meta = (Bindwidget))
		class UTextBlock* Text_Name;

	UPROPERTY(VisibleAnywhere, meta = (Bindwidget))
		class UProgressBar* ProgressBar_HP;

//public:
//	UPROPERTY(EditAnywhere, Category ="Image")
//		UImage*

public:
	UCUI_TargetInfo(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetLevelText(FString Instring);
	void SetLevelName(FString Instring);

private:
	UTexture2D* ProgressBar_HP_Border_Wait;
	FSlateBrush Border_WaitBrush;

	UTexture2D* ProgressBar_Hp_Border_Battle;
	FSlateBrush Border_BattleBrush;
};
