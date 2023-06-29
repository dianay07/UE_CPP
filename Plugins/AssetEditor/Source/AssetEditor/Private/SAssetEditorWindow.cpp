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

	// �����Ϻ� �⺻������ ����
	{
		FDetailsViewArgs args(false, false, true, FDetailsViewArgs::HideNameArea);
		args.ViewIdentifier = "AssetEditorDetailsView";
		DetailsView = prop.CreateDetailView(args);

		// FOnGetDetailCustomizationInstance�� �߰��� Ŀ���� â�� ���
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
				// ��������
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				// ��������
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
		// â�� ���� �����ϰٴ� �ҷ��� �����Ϻ� �ȿ� �ּ��� �����϶�� ���
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);

		// �� â������ Ŀ������ â�� �������� ����
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

	// ���� �������� �ּ� ����
	if (!!GetEditingObject())
		RemoveEditingObject(GetEditingObject());

	AddEditingObject(InDataPtr->Asset);				// �ٽ� ������ â ���
	DetailsView->SetObject(InDataPtr->Asset);		// â�� ������ �信 ������ �ּ� ���
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
