 #include "SAssetCheckBoxes.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SAssetDetailsView.h"
#include "JobEditorStyle.h"

void SAssetCheckBoxes::AddProperties(TSharedPtr<IPropertyHandle> InHandle)
{
	uint32 number = 0;
	InHandle->GetNumChildren(number);

	for (uint32 i = 0; i < number; i++)
		InternalDatas.Add(FInternalData(InHandle->GetChildHandle(i)));
}

TSharedRef<SWidget> SAssetCheckBoxes::Draw(bool bBackGround)
{
	TSharedPtr<SUniformGridPanel> panel;
	SAssignNew(panel, SUniformGridPanel);
	panel->SetMinDesiredSlotWidth(200);

	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		panel->AddSlot(i % 5, i / 5)
			[
				SNew(SCheckBox)
				.IsChecked(InternalDatas[i].bChecked)				// 체크박스가 체크되어있는지 ?
			.OnCheckStateChanged(this, &SAssetCheckBoxes::OnCheckStateChanged, i)
			[
				SNew(STextBlock)
				.Text(FText::FromString(InternalDatas[i].Name))
			]
			];
	}

	if (bBackGround == false)
		return panel.ToSharedRef();

	TSharedPtr<SBorder> border = SNew(SBorder)
		.BorderImage(FJobEditorStyle::Get()->Array_Image.Get())
		[
			panel.ToSharedRef()
		];

	return  border.ToSharedRef();
}

void SAssetCheckBoxes::DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder* InChildrenBuilder)
{
	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		if(InternalDatas[i].bChecked == false)
			continue;

		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
		IDetailPropertyRow& row = InChildrenBuilder->AddProperty(handle.ToSharedRef());

		FString name = FString("Name ") + FString::FromInt(i + 1);

		row.CustomWidget()
			.NameContent()
			[
				handle->CreatePropertyNameWidget()
			]
		.ValueContent()
			.MinDesiredWidth(FJobEditorStyle::Get()->DesiredWidth.X)
			.MaxDesiredWidth(FJobEditorStyle::Get()->DesiredWidth.Y)
			[
				handle->CreatePropertyValueWidget()
			];
	}
}

void SAssetCheckBoxes::SetUtilities(TSharedPtr<IPropertyUtilities> InUtilities)
{
	Utilities = InUtilities;
}

void SAssetCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int32 InIndex)
{
	InternalDatas[InIndex].bChecked = !InternalDatas[InIndex].bChecked;


	SAssetDetailsView::OnRefreshByCheckBoxes();
	{
		Utilities->ForceRefresh();
	}
	SAssetDetailsView::OffRefreshByCheckBoxes();
}

bool SAssetCheckBoxes::CanDraw(TSharedPtr<IPropertyHandle> InHandle, int InCount)
{
	bool bCheck = true;
	bCheck &= InCount > 0;

	int32 index = InHandle->GetIndexInArray();
	bCheck &= index >= 0;
	bCheck &= index < InCount;

	return bCheck;
}

void SAssetCheckBoxes::CheckDefaultObject(int32 InIndex, UObject* InValue)
{
	UObject* val = nullptr;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (!!val && InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SAssetCheckBoxes::CheckDefaultValue(int32 InIndex, int InValue)
{
	int val = 0;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SAssetCheckBoxes::CheckDefaultValue(int32 InIndex, float InValue)
{
	float val = 0.0f;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SAssetCheckBoxes::CheckDefaultValue(int32 InIndex, bool InValue)
{
	bool val = false;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SAssetCheckBoxes::CheckDefaultValue(int32 InIndex, const FVector& InValue)
{
	FVector val = FVector::ZeroVector;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SAssetCheckBoxes::CheckDefaultValue(int32 InIndex, const FText& InValue)
{
	FText val = FText::FromString("");
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue.CompareTo(val) == false)
		InternalDatas[InIndex].bChecked = true;
}

void SAssetCheckBoxes::CheckDefaultValue(int32 InIndex, uint8 InType)
{
	uint8 Type = 0;
	InternalDatas[InIndex].Handle->GetValue(Type);

	if (InType != Type)
		InternalDatas[InIndex].bChecked = true;
}