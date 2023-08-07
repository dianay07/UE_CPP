#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Job/CJobStructure.h"
#include "CUI_SkillIcon.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_SkillIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		FText Name;

public:
};
