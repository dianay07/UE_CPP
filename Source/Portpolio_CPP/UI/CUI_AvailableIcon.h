#pragma once

#include "CoreMinimal.h"
#include "CUIBaseClass.h"
#include "IAvailable.h"
#include "Blueprint/UserWidget.h"
#include "Job/CJobStructure.h"
#include "CUI_AvailableIcon.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_AvailableIcon : public UCUIBaseClass
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class USizeBox* Icon_SizeBox;

	UPROPERTY(meta = (BindWidget))
		class UImage* Icon_Image;

	UPROPERTY(EditAnywhere)
		FText Name;

	UPROPERTY()
		EAvailableType AvailableType;

	UPROPERTY()
		int32 AvailableIndex;

	UPROPERTY()
		FSkillData Data;

public:
	void SetSkillData(const FSkillData& InData);
};
