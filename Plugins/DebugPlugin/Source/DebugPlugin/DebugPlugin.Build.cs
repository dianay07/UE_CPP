// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DebugPlugin : ModuleRules
{
	public DebugPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");

        PrivateDependencyModuleNames.Add("Portpolio_CPP");

        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("Slate");
        PrivateDependencyModuleNames.Add("SlateCore");
        PrivateDependencyModuleNames.Add("GameplayDebugger");
        PrivateDependencyModuleNames.Add("DesktopPlatform");
        PrivateDependencyModuleNames.Add("MainFrame");
        PrivateDependencyModuleNames.Add("EditorStyle");
        PrivateDependencyModuleNames.Add("UnrealEd");
    }
}
