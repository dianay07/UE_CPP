#include "DebugPlugin.h"

#include "DebugCategory.h"
#include "GameplayDebugger.h"

#define LOCTEXT_NAMESPACE "FDebugPluginModule"

IMPLEMENT_MODULE(FDebugPluginModule, DebugPlugin)

void FDebugPluginModule::StartupModule()
{
	//Debugger
	{
		IGameplayDebugger::FOnGetCategory category;
		category.BindStatic(&FDebugCategory::MakeInstance);

		IGameplayDebugger::Get().RegisterCategory("PlayerDebug", category, EGameplayDebuggerCategoryState::EnabledInGameAndSimulate, 5);
		IGameplayDebugger::Get().NotifyCategoriesChanged();
	}
}

void FDebugPluginModule::ShutdownModule()
{
	if (IGameplayDebugger::IsAvailable())
		IGameplayDebugger::Get().UnregisterCategory("PlayerDebug");
}

#undef LOCTEXT_NAMESPACE