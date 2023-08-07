#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_TargetingCursor.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_TargetingCursor : public UUserWidget
{
	GENERATED_BODY()

public:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class USizeBox* SizeBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget))
		class UImage* Image;*/

	UPROPERTY(meta=(BindWidget))
		class USizeBox* Box;

	UPROPERTY(meta=(BindWidget))
		class UImage* Image;

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
