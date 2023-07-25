#include "SJobHitData.h"

#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "SAssetCheckBoxes.h"
#include "JobEditorStyle.h"
#include "DetailWidgetRow.h"

TArray<TSharedPtr<SAssetCheckBoxes>> SJobHitData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SJobHitData::MakeInstance()
{
	return MakeShareable(new SJobHitData());
}

TSharedPtr<SAssetCheckBoxes> SJobHitData::AddCheckBoxes()
{
	TSharedPtr<SAssetCheckBoxes> checkBoxes = MakeShareable(new SAssetCheckBoxes());
	int32 index = CheckBoxes.Add(checkBoxes);

	return CheckBoxes[index];
}

void SJobHitData::EmptyCheckBoxes()
{
	for (TSharedPtr<SAssetCheckBoxes> ptr : CheckBoxes)
	{
		if (ptr.IsValid())
			ptr.Reset();
	}

	CheckBoxes.Empty();
}

void SJobHitData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (SAssetCheckBoxes::CanDraw(InPropertyHandle, CheckBoxes.Num()) == false)
	{
		InHeaderRow
			.NameContent()
			[
				InPropertyHandle->CreatePropertyNameWidget()
			]
		.ValueContent()
			.MinDesiredWidth(FJobEditorStyle::Get()->DesiredWidth.X)
			.MaxDesiredWidth(FJobEditorStyle::Get()->DesiredWidth.Y)
			[
				InPropertyHandle->CreatePropertyValueWidget()
			];

		return;
	}


	int32 index = InPropertyHandle->GetIndexInArray();
	CheckBoxes[index]->SetUtilities(InCustomizationUtils.GetPropertyUtilities());

	FString name = InPropertyHandle->GetPropertyDisplayName().ToString();
	name = "Hit Data - " + name;

	InHeaderRow
		.NameContent()
		[
			SNew(SBorder)
			.BorderImage(FJobEditorStyle::Get()->Array_Image.Get())
		[
			InPropertyHandle->CreatePropertyNameWidget(FText::FromString(name))
		]
		]
	.ValueContent()
		.MinDesiredWidth(FJobEditorStyle::Get()->DesiredWidth.X)
		.MaxDesiredWidth(FJobEditorStyle::Get()->DesiredWidth.Y)
		[
			CheckBoxes[index]->Draw(true)
		];
}

void SJobHitData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (SAssetCheckBoxes::CanDraw(InPropertyHandle, CheckBoxes.Num()) == false)
	{
		uint32 number = 0;
		InPropertyHandle->GetNumChildren(number);

		for (uint32 i = 0; i < number; i++)
		{
			TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
			IDetailPropertyRow& row = InChildBuilder.AddProperty(handle.ToSharedRef());

			TSharedPtr<SWidget> name;
			TSharedPtr<SWidget> value;

			row.GetDefaultWidgets(name, value);

			row.CustomWidget()
				.NameContent()
				[
					name.ToSharedRef()
				]
			.ValueContent()
				.MinDesiredWidth(FJobEditorStyle::Get()->DesiredWidth.X)
				.MaxDesiredWidth(FJobEditorStyle::Get()->DesiredWidth.Y)
				[
					value.ToSharedRef()
				];
		}//for(i)

		return;
	}

	int32 index = InPropertyHandle->GetIndexInArray();
	CheckBoxes[index]->DrawProperties(InPropertyHandle, &InChildBuilder);
}