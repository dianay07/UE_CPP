#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class ASSETEDITOR_API FAssetEditorCommand
	: public TCommands<FAssetEditorCommand>
{
public:
	FAssetEditorCommand();
	~FAssetEditorCommand();

	void StartUp();

public:
	void RegisterCommands() override;

private:
	TSharedPtr<FExtender> Extender;

	TSharedPtr<FUICommandList> Command;
	TSharedPtr<FUICommandInfo> Id;

private:
	void AddToolBar(FToolBarBuilder& InBuilder);
	void OnClicked();
};
