#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STableRow.h"

struct FAssetRowData
{
	//union AssetType
	//{
	//	class UCJobDataAsset* JobAsset;
	//};

	int Number;
	FString Name;
	//AssetType* Type;
	class UCJobDataAsset* Asset;

	FAssetRowData()
	{
		
	}

	FAssetRowData(int32 InNumber, FString InName, class UCJobDataAsset* InAsset)
		: Number(InNumber), Name(InName), Asset(InAsset)
	{
		
	}

	static TSharedPtr<FAssetRowData> Make(int32 InNumber, FString InName, class UCJobDataAsset* InAsset)
	{
		return MakeShareable(new FAssetRowData(InNumber, InName, InAsset));
	}
};

typedef TSharedPtr<FAssetRowData> FAssetRowDataPtr;
///////////////////////////////////////////////////////////////////////////////

class ASSETEDITOR_API SAssetTableRow
	: public SMultiColumnTableRow<FAssetRowDataPtr>
{
public:
	SLATE_BEGIN_ARGS(SAssetTableRow) {}
	SLATE_ARGUMENT(FAssetRowDataPtr, RowData)		// 변수 정의
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;


private:
	// 데이터 만들어서 전달
	FAssetRowDataPtr Data;

};

//////////////////////////////////////////////////////////////////////////////////

DECLARE_DELEGATE_OneParam(FOnAssetListViewSelectedItem, FAssetRowDataPtr);

class ASSETEDITOR_API SAssetEditorLeftArea
	: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetEditorLeftArea) {}
	SLATE_EVENT(FOnAssetListViewSelectedItem, OnSelectedItem)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

	bool HasRowDataptr() { return RowDatas.Num() > 0; }
	FAssetRowDataPtr GetFirstDataPtr() { return RowDatas[0]; }

	// TODO :: JobData Type 에셋만 받는거 어캐
	void SelectDataPtr(class UCJobDataAsset* InAsset);

private:
	TSharedRef<ITableRow> OnGenerateRow(FAssetRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);
	void OnSelectionChanged(FAssetRowDataPtr InDataPtr, ESelectInfo::Type InType);

	FText OnGetAssetCount() const;

	void OnTextChanged(const FText& InText);
	void OnTextComitted(const FText& InText, ETextCommit::Type InType);

private:
	void ReadDataAssetList();

private:
	FOnAssetListViewSelectedItem OnListViewSeletedItem;

private:
	TArray<FAssetRowDataPtr> RowDatas;
	TSharedPtr<SListView<FAssetRowDataPtr>> ListView;

private:
	TSharedPtr<class SSearchBox> SearchBox;
	FText SearchText;
};
