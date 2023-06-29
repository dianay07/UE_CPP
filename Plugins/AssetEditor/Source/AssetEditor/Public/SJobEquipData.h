#pragma once

#include "CoreMinimal.h"

class ASSETEDITOR_API SJobEquipData
	: public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	static TSharedPtr<class SAssetCheckBoxes> CreateCheckBoxes();

	void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;
	void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

private:
	static TSharedPtr<class SAssetCheckBoxes> CheckBoxes;
};
