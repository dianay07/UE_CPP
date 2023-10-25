#pragma once

#include "CoreMinimal.h"
#include "CUIBaseClass.h"
#include "IAvailable.h"
#include "Blueprint/UserWidget.h"
#include "Job/CJobStructure.h"
#include "CUI_Slot.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_Slot : public UCUIBaseClass
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
		int32 ItemIndex = 999;

	FTimerHandle availableTimerHandle;

public:
	FORCEINLINE bool GetCanUse() { return isCanUse; }
	FORCEINLINE void SetCanUse(bool value) { isCanUse = value; }

private:
	FSkillData Data;
	bool isCanUse = true;


protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void Reset();

};

inline void UCUI_Slot::Reset()
{
	if (isCanUse == false) isCanUse = true;

	availableTimerHandle.Invalidate();
}

