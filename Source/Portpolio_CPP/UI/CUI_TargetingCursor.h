#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_TargetingCursor.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_TargetingCursor : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		class UImage* Image;

protected:
	virtual void NativeOnInitialized() override;
	
};
