#pragma once

#include "CoreMinimal.h"
#include "Component/CStatusComponent.h"
#include "UI/CUIBaseClass.h"
#include "CUI_Status.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCUI_Status : public UCUIBaseClass
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ProgressBar_Hp;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Hp;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ProgressBar_Mp;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Mp;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ProgressBar_EXP;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_EXP;

public:
	UCUI_Status(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	class ACPlayer* OwnerPlayer;
	class UCStatusComponent* Status;


};

