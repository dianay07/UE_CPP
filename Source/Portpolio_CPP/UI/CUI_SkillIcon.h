#pragma once

#include "CoreMinimal.h"
#include "CUIBaseClass.h"
#include "Blueprint/UserWidget.h"
#include "Job/CJobStructure.h"
#include "CUI_SkillIcon.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_SkillIcon : public UCUIBaseClass
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
		int32 SkillIndex;

public:
	void SetSkillData(const FSkillData& InData);

private:
	FSkillData Data;
};
