#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Job/CJobStructure.h"
#include "CUI_SkillIcon.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_SkillIcon : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		class USizeBox* IconSize;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		class UImage* IconImage;

public:
	UCUI_SkillIcon(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	void SetSkillData(FSkillData InSkillData);

private:
	FSkillData SkillData;

};
