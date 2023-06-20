#include "AssetEditorWindow.h"
#include "SAssetEditorLeftArea.h"
#include "Job/CJobDataAsset.h"

const FName FAssetEditorWindow::EditorName = "AssetEditorWindow";
const FName FAssetEditorWindow::LeftAreaTabId = "LeftArea";
const FName FAssetEditorWindow::DetailTabId = "Details";

TSharedPtr<FAssetEditorWindow> FAssetEditorWindow::Instance = nullptr;

void FAssetEditorWindow::OpenWindow(FString InAssetName)
{
	if(Instance.IsValid())
	{
		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}

	Instance = MakeShareable(new FAssetEditorWindow());
	Instance->Open(InAssetName);
}

void FAssetEditorWindow::Shutdown()
{
	if(Instance.IsValid())
	{
		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}
}

void FAssetEditorWindow::Open(FString InAssetName)
{
	LeftArea = SNew(SAssetEditorLeftArea);

	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("AssetEditorWindow_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				// 고정공간
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				// 가변공간
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.175f)
					->AddTab(LeftAreaTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.725f)
					->AddTab(DetailTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
			)
		);

	UCJobDataAsset* asset = NewObject<UCJobDataAsset>();
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, true, true, asset);
}

void FAssetEditorWindow::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab tab;
	tab.BindSP(this, &FAssetEditorWindow::Spawn_ListViewTab);
	TabManager->RegisterTabSpawner(LeftAreaTabId, tab);
}

TSharedRef<SDockTab> FAssetEditorWindow::Spawn_ListViewTab(const FSpawnTabArgs& InArgs)
{
	/*TSharedPtr<SDockTab> tab = SNew(SDockTab)
		[
			SNew(SButton)
			.OnClicked(this, &FAssetEditorWindow::OnClicked)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Test"))
			]
		];

	return tab.ToSharedRef();*/
	return SNew(SDockTab)
	[
		LeftArea.ToSharedRef()
	];
}

FName FAssetEditorWindow::GetToolkitFName() const
{
	return EditorName;
}

FText FAssetEditorWindow::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);
}

FString FAssetEditorWindow::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString();
}

FLinearColor FAssetEditorWindow::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1);
}

FReply FAssetEditorWindow::OnClicked()
{
	return FReply::Handled();
}
