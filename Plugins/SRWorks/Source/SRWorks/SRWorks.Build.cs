// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class SRWorks : ModuleRules
{
    public SRWorks(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        //Type = ModuleType.External;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
            string SRWorksDirectory = Path.Combine(BaseDirectory, "Binaries/Win64");

            // Add the import library
            PublicLibraryPaths.Add(SRWorksDirectory);
            PublicAdditionalLibraries.Add("SRWork_Client_API.lib");

            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add("SRWork_Client_API.dll");
            string[] dlls = Directory.GetFiles(SRWorksDirectory, "*.dll");
            for (int i = 0; i < dlls.Length; i++)
            {
                if (dlls[i] == "SRWork_Client_API.dll") continue;
                RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(SRWorksDirectory, dlls[i])));
            }
            string[] axs = Directory.GetFiles(SRWorksDirectory, "*.ax");
            for (int i = 0; i < axs.Length; i++)
            {
                RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(SRWorksDirectory, axs[i])));
            }
        }

        PublicIncludePaths.AddRange(
			new string[] {
				"SRWorks/Public",
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"SRWorks/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RHI",
                "RenderCore",
                "Projects",
                "ProceduralMeshComponent",
                "XmlParser",
                "D3D11RHI"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
                "Slate",
                "SlateCore",
                "ProceduralMeshComponent"
            }
            );
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
            }
			);

        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }
    }
}
