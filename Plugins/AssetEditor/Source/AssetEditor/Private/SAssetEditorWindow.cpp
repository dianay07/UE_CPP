#include "SAssetEditorWindow.h"

#include "SAssetDetailsView.h"
#include "SAssetEditorLeftArea.h"
#include "SJobEquipData.h"
#include "SJobSkillData.h"
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
	LeftArea = SNew(SAssetEditorLeftArea)
		.OnSelectedItem(this, &FAssetEditorWindow::OnListViewSelectedItem);

	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// 디테일뷰 기본형으로 생성
	{
		FDetailsViewArgs args(false, false, true, FDetailsViewArgs::HideNameArea);
		args.ViewIdentifier = "AssetEditorDetailsView";
		DetailsView = prop.CreateDetailView(args);

		// FOnGetDetailCustomizationInstance를 추가시 커스텀 창을 출력
		FOnGetDetailCustomizationInstance detailView;
		detailView.BindStatic(&SAssetDetailsView::MakeInstance);
		DetailsView->SetGenericLayoutDetailsDelegate(detailView);
	}

	// EquipData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SJobEquipData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("EquipData", instance);
	}

	// Skill Data
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SJobSkillData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("SkillData", instance);
	}

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

	UCJobDataAsset* asset = nullptr;
	asset = LeftArea->GetFirstDataPtr()->Asset;

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, true, true, asset);
	//DetailsView->SetObject(asset);
	LeftArea->SelectDataPtr(asset);
}

bool FAssetEditorWindow::OnRequestClose()
{
	if(!!DetailsView)
	{
		// 창을 열때 편집하겟다 불러온 디테일뷰 안에 애셋을 해제하라는 명령
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);

		// 이 창에서만 커스텀한 창이 나오도록 해제
		if(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			prop.UnregisterCustomPropertyTypeLayout("EquipData");
			prop.UnregisterCustomPropertyTypeLayout("SkillData");
		}
	}

	if (LeftArea.IsValid())
		LeftArea.Reset();

	if (DetailsView.IsValid())
		DetailsView.Reset();

	return true;
}

void FAssetEditorWindow::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab tab;
	tab.BindSP(this, &FAssetEditorWindow::Spawn_LeftAreaTab);
	TabManager->RegisterTabSpawner(LeftAreaTabId, tab);

	FOnSpawnTab tab2;
	tab2.BindSP(this, &FAssetEditorWindow::Spawn_DetailsViewTab);
	TabManager->RegisterTabSpawner(DetailTabId, tab2);
}

TSharedRef<SDockTab> FAssetEditorWindow::Spawn_LeftAreaTab(const FSpawnTabArgs& InArgs)
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

TSharedRef<SDockTab> FAssetEditorWindow::Spawn_DetailsViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			DetailsView.ToSharedRef()
		];
}

void FAssetEditorWindow::OnListViewSelectedItem(FAssetRowDataPtr InDataPtr)
{
	if (InDataPtr == nullptr)
		return;

	// 현재 편집중인 애셋 제거
	if (!!GetEditingObject())
		RemoveEditingObject(GetEditingObject());

	AddEditingObject(InDataPtr->Asset);				// 다시 편집할 창 등록
	DetailsView->SetObject(InDataPtr->Asset);		// 창의 디테일 뷰에 보여질 애셋 등록
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
