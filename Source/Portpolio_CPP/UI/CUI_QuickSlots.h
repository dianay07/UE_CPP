#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_QuickSlots.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_QuickSlots : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UUniformGridPanel* Slots;

};
