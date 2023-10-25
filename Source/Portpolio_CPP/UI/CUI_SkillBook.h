#pragma once

#include "CoreMinimal.h"
#include "Job/CJobStructure.h"
#include "Blueprint/UserWidget.h"
#include "CUI_SkillBook.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_SkillBook : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UOverlay* Book_Overlay;

	UPROPERTY(meta = (BindWidget))
		class UImage* BackGround;

	UPROPERTY(meta = (BindWidget))
		class USizeBox* Book_SizeBox;

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* Book_ScrollBox;

	UPROPERTY(EditAnywhere)
		TArray<class UCUI_AvailableIcon*> Icons;

	UPROPERTY()
		TSubclassOf<UCUI_AvailableIcon> IconClass;

public:
	UCUI_SkillBook(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void RefreshSkillData();

private:
	class ACPlayer* OwnerPlayer;

	TArray<FSkillData> SkillDatas;
};
