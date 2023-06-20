#include "SAssetEditorLeftArea.h"

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

	return SNew(STextBlock)
		.Text(FText::FromString(str));
}

///////////////////////////////////////////////////////////////////////////////

void SAssetEditorLeftArea::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1)
		[
			SAssignNew(ListView, SListView<FAssetRowDataPtr>)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Number")
				.DefaultLabel(FText::FromString("Number"))
				+ SHeaderRow::Column("Name")
				.DefaultLabel(FText::FromString("Name"))
			)
		.ListItemsSource(&RowDatas)
		.OnGenerateRow(this, &SAssetEditorLeftArea::OnGenerateRow)
		]
	];
}

// RowDatas의 갯수만큼 콜
TSharedRef<ITableRow> SAssetEditorLeftArea::OnGenerateRow(FAssetRowDataPtr InRow,
	const TSharedRef<STableViewBase>& InTable)
{
	return SNew(SAssetTableRow, InTable)
		.RowData(InRow);
}
