#pragma once

#include "CoreMinimal.h"
#include "Job/CJobStructure.h"
#include "Blueprint/UserWidget.h"
#include "CUI_SkillBook.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_SkillBook : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		class UUniformGridPanel* UniformGridPanel;

	UPROPERTY(VisibleAnywhere)
		TArray<FSkillData> SkillDatas;

public:
	UCUI_SkillBook(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

private:
	class ACPlayer* OwnerPlayer;
};
