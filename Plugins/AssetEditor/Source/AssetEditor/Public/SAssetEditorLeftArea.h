#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STableRow.h"

struct FAssetRowData
{
	union AssetType
	{
		class UCJobDataAsset* JobAsset;
	};

	int Number;
	FString Name;
	AssetType Type;

	FAssetRowData()
	{
		
	}

	FAssetRowData(int32 InNumber, FString InName, AssetType InType)
		: Number(InNumber), Name(InName), Type(InType)
	{
		
	}

	TSharedPtr<FAssetRowData> Make(int32 InNumber, FString InName, AssetType InType)
	{
		return MakeShareable(new FAssetRowData(InNumber, InName, InType));
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

class ASSETEDITOR_API SAssetEditorLeftArea
	: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetEditorLeftArea) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<ITableRow> OnGenerateRow(FAssetRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);

private:
	TArray<FAssetRowDataPtr> RowDatas;
	TSharedPtr<SListView<FAssetRowDataPtr>> ListView;
};
