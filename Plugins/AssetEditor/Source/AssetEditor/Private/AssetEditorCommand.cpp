#include "AssetEditorCommand.h"

#include "JobEditorStyle.h"
#include "AssetEditorWindow.h"
#include "LevelEditor.h"

FAssetEditorCommand::FAssetEditorCommand()
	: TCommands("Toolbar_Buttons", FText::FromString(""), NAME_None, FEditorStyle::GetStyleSetName())
{
	Command = MakeShareable(new FUICommandList());
}

FAssetEditorCommand::~FAssetEditorCommand()
{
	if (Command.IsValid())
		Command.Reset();

	if (Extender.IsValid())
		Extender.Reset();
}

void FAssetEditorCommand::StartUp()
{
	FAssetEditorCommand::RegisterCommands();

	Extender = MakeShareable(new FExtender());

	FToolBarExtensionDelegate toolbar = FToolBarExtensionDelegate::CreateRaw(this, &FAssetEditorCommand::AddToolBar);
	Extender->AddToolBarExtension("Settings", EExtensionHook::After, Command, toolbar);

	FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	levelEditor.GetToolBarExtensibilityManager()->AddExtender(Extender);
}

void FAssetEditorCommand::RegisterCommands()
{
#define LOCTEXT_NAMESPACE ""
		UI_COMMAND(Id, "Weapon", "", EUserInterfaceActionType::Button, FInputChord());
#undef LOCTEXT_NAMESPACE


	FExecuteAction action;
	action.BindRaw(this, &FAssetEditorCommand::OnClicked);

	Command->MapAction(Id, action, FCanExecuteAction());
}

void FAssetEditorCommand::AddToolBar(FToolBarBuilder& InBuilder)
{
	FString name = TEXT("잡 생성");

	InBuilder.AddSeparator();
	InBuilder.AddToolBarButton(Id, NAME_None, FText::FromString(name), FText::FromString("Job Asset Editor"), FJobEditorStyle::Get()->ToolBar_Icon, NAME_None);
}

void FAssetEditorCommand::OnClicked()
{
	FAssetEditorWindow::OpenWindow();
}
