#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAssetEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<class FAssetContextMenu> ContextMenu;
	TSharedPtr<class FAssetEditorCommand> Command;
};
