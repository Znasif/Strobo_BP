// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;

public class SRanipalLip : ModuleRules
{
	public SRanipalLip(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                Path.GetFullPath(Path.Combine(ModuleDirectory, ".."))+"/SRanipalLip/Public",
                Path.GetFullPath(Path.Combine(ModuleDirectory, ".."))+"/SRanipal/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                Path.GetFullPath(Path.Combine(ModuleDirectory, ".."))+"/SRanipalLip/Private",
                Path.GetFullPath(Path.Combine(ModuleDirectory, ".."))+"/SRanipal/Private",
                Path.GetFullPath(Path.Combine(ModuleDirectory, ".."))+"/SRanipal/Public/Eye",
                Path.GetFullPath(Path.Combine(ModuleDirectory, ".."))+"/SRanipal/Public/Lip"
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "SRanipal"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
