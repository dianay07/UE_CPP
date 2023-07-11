#include "SJobEquipData.h"
#include "SAssetCheckBoxes.h"
#include "JobEditorStyle.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"

TSharedPtr<class SAssetCheckBoxes> SJobEquipData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SJobEquipData::MakeInstance()
{
	return MakeShareable(new SJobEquipData());
}

TSharedPtr<SAssetCheckBoxes> SJobEquipData::CreateCheckBoxes()
{
	if (CheckBoxes.IsValid())
	{
		CheckBoxes.Reset();
		CheckBoxes = nullptr;
	}

	return CheckBoxes = MakeShareable(new SAssetCheckBoxes());
}

void SJobEquipData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow,
                                    IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	CheckBoxes->SetUtilities(InCustomizationUtils.GetPropertyUtilities());

	InHeaderRow
		.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MinDesiredWidth(FJobEditorStyle::Get()->DesiredWidth.X)
		.MaxDesiredWidth(FJobEditorStyle::Get()->DesiredWidth.Y)
		[
			CheckBoxes->Draw()
		];
}

void SJobEquipData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder,
	IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	CheckBoxes->DrawProperties(InPropertyHandle, &InChildBuilder);
}
