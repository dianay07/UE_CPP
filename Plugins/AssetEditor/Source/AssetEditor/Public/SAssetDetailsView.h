#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class ASSETEDITOR_API SAssetDetailsView
	: public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

public:
	static void OnRefreshByCheckBoxes() { bRefreshByCheckBoxes = true; }
	static void OffRefreshByCheckBoxes() { bRefreshByCheckBoxes = false; }

private:
	static bool bRefreshByCheckBoxes;
};
