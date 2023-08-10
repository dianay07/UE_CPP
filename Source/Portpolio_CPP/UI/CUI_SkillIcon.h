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
	UPROPERTY(meta = (BindWidget))
		class USizeBox* Icon_SizeBox;

	UPROPERTY(meta = (BindWidget))
		class UImage* Icon_Image;

	UPROPERTY(EditAnywhere)
		FText Name;

public:
	UCUI_SkillIcon(const FObjectInitializer& ObjectInitializer);
	UCUI_SkillIcon(const FObjectInitializer& ObjectInitializer, const FSkillData& InData);

	void NativeConstruct() override;

public:
	void SetSkillData(const FSkillData& InData);

private:
	FSkillData Data;

};
