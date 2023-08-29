#include "SAssetEditorLeftArea.h"

#include "Job/CJobAsset.h"
#include "EngineUtils.h"
#include "Widgets/Input/SSearchBox.h"

void SAssetTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	Data = InArgs._RowData;

	SMultiColumnTableRow<FAssetRowDataPtr>::Construct
	(
		FSuperRowType::FArguments().Style(FEditorStyle::Get(), "TableView.DarkRow"), InOwnerTable
	);
}

TSharedRef<SWidget> SAssetTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	FString str;
	if (InColumnName == "Number")
		str = FString::FromInt(Data->Number);
	else if (InColumnName == "Name")
		str = Data->Name;
	//else if(InColumnName == "AssetType")
	//{
	//	str = typeid(Data->Type).name();
	//}

	return SNew(STextBlock)
		.Text(FText::FromString(str));
}

///////////////////////////////////////////////////////////////////////////////

void SAssetEditorLeftArea::Construct(const FArguments& InArgs)
{
	OnListViewSeletedItem = InArgs._OnSelectedItem;

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(2, 0)
			[
				SAssignNew(SearchBox, SSearchBox)
				.SelectAllTextWhenFocused(true)
				.OnTextChanged(this, &SAssetEditorLeftArea::OnTextChanged)
				.OnTextCommitted(this, &SAssetEditorLeftArea::OnTextComitted)
			]
			+ SVerticalBox::Slot()
		.FillHeight(1)
		[
			SAssignNew(ListView, SListView<FAssetRowDataPtr>)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Number")
				.DefaultLabel(FText::FromString(""))
				.ManualWidth(40)
				+ SHeaderRow::Column("Name")
				.DefaultLabel(FText::FromString("Name"))
				//+ SHeaderRow::Column("AssetType")
				//.DefaultLabel(FText::FromString("Asset Type"))
			)
		.ListItemsSource(&RowDatas)
		.OnGenerateRow(this, &SAssetEditorLeftArea::OnGenerateRow)
		.OnSelectionChanged(this, &SAssetEditorLeftArea::OnSelectionChanged)
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Right)
		.Padding(FMargin(8, 2))
		[
			SNew(STextBlock)
			.Text(this, &SAssetEditorLeftArea::OnGetAssetCount)
		]
	];

	ReadDataAssetList();
}

// RowDatas의 갯수만큼 콜
TSharedRef<ITableRow> SAssetEditorLeftArea::OnGenerateRow(FAssetRowDataPtr InRow,
	const TSharedRef<STableViewBase>& InTable)
{
	return SNew(SAssetTableRow, InTable) 
		.RowData(InRow);
}

void SAssetEditorLeftArea::OnSelectionChanged(FAssetRowDataPtr InDataPtr, ESelectInfo::Type InType)
{
	if (InDataPtr.IsValid() == false)
		return;

	OnListViewSeletedItem.ExecuteIfBound(InDataPtr);
}

FAssetRowDataPtr SAssetEditorLeftArea::GetRowDataPtrByName(FString InAssetName)
{
	for (FAssetRowDataPtr ptr : RowDatas)
	{
		if (ptr->Name == InAssetName)
			return ptr;
	}

	return nullptr;
}

FString SAssetEditorLeftArea::SelectedRowDataPtrName()
{
	TArray<FAssetRowDataPtr> ptrs = ListView->GetSelectedItems();

	if (ptrs.Num() > 0)
		return ptrs[0]->Asset->GetName();

	return "";
}

void SAssetEditorLeftArea::SelectDataPtr(UCJobAsset* InAsset)
{
	if (HasRowDataptr() == false)
		return;

	for(FAssetRowDataPtr ptr : RowDatas)
	{
		if(ptr->Asset == InAsset)
		{
			// 선택이 바뀌면서 OnSelectionChanged 이벤트 호출
			ListView->SetSelection(ptr);

			return;
		}
	}
}

FText SAssetEditorLeftArea::OnGetAssetCount() const
{
	FString str = FString::Printf(L"%d", RowDatas.Num());

	return FText::FromString(str);
}

void SAssetEditorLeftArea::OnTextChanged(const FText& InText)
{
	if (SearchText.CompareToCaseIgnored(InText) == 0)
		return;

	SearchText = InText;
	ReadDataAssetList();
}

void SAssetEditorLeftArea::OnTextComitted(const FText& InText, ETextCommit::Type InType)
{
	GLog->Log(InText.ToString());
	GLog->Log(StaticEnum<ETextCommit::Type>()->GetValueAsString(InType));

	/*switch (InType)
	{
	case ETextCommit::OnCleared :

	}*/ 

	OnTextChanged(InText);
}

void SAssetEditorLeftArea::ReadDataAssetList()
{
	RowDatas.Reset();

	TArray<UObject*> objects;
	EngineUtils::FindOrLoadAssetsByPath("/Game/08_Weapon/", objects, EngineUtils::ATL_Regular);

	int32 index = 0;
	for(UObject* obj : objects)
	{
		UCJobAsset* asset = Cast<UCJobAsset>(obj);

		if(asset == nullptr)
			continue;

		FString name = asset->GetName();
		if(SearchText.IsEmpty() == false)
		{
			if(name.Contains(SearchText.ToString()) == false)
				continue;
		}

		RowDatas.Add(FAssetRowData::Make(++index, name, asset));
	}

	RowDatas.Sort([](const FAssetRowDataPtr& A, const FAssetRowDataPtr& B)
		{
			return A->Number < B->Number;
		});

	ListView->RequestListRefresh();
}
