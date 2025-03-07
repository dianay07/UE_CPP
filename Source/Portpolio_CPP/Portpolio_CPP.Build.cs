// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Portpolio_CPP : ModuleRules
{
	public Portpolio_CPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "UMG",
            "AIModule", "GameplayTasks", "NavigationSystem",
            "EngineSettings" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore"});
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
