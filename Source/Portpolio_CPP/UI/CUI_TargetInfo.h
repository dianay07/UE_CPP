#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_TargetInfo.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_TargetInfo : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USizeBox* InfoBox;

	UPROPERTY(VisibleAnywhere)
		class UTextBlock* Text_Level;

	UPROPERTY(VisibleAnywhere)
		class UTextBlock* Text_Name;

public:
	virtual void NativeConstruct() override;

	void SetLevelText(FString Instring);
	void SetLevelName(FString Instring);
};
