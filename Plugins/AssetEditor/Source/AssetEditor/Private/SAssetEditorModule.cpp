#include "SAssetEditorModule.h"

#include "SAssetContextMenu.h"
#include "SAssetEditorCommand.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "JobEditorStyle.h"

#define LOCTEXT_NAMESPACE "FAssetEditorModule"
IMPLEMENT_MODULE(FAssetEditorModule, AssetEditor)

void FAssetEditorModule::StartupModule()
{
	FJobEditorStyle::Get();

	IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type categories = assetTools.RegisterAdvancedAssetCategory("Job Asset", FText::FromString("Create Custom Asset"));

	ContextMenu = MakeShareable(new FAssetContextMenu(categories));
	assetTools.RegisterAssetTypeActions(ContextMenu.ToSharedRef());

	Command = MakeShareable(new FAssetEditorCommand());
	Command->StartUp();
}

void FAssetEditorModule::ShutdownModule()
{
	if (ContextMenu.IsValid())
		ContextMenu.Reset();

	if (Command.IsValid())
		Command.Reset();

	FJobEditorStyle::Shutdown();
}

#undef LOCTEXT_NAMESPACE