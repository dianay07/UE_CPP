#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHUDLayout.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCHUDLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UCanvasPanel* BaseCanvasPanel;

	UPROPERTY()
		class UCUI_SkillBook* SkillBookWidget;

	UPROPERTY()
		class UCUI_QuickSlots* QuickSlots;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UCUI_SkillBook> UI_SkillBookClass;

protected:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
