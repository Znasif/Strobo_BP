// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SRWorks.h"
#include "Core.h"
#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"
#include "SRWork_Client_API.h"
#include "ViveSR_PassThrough.h"
#include "ViveSR_PassThrough4K.h"
#include "ViveSR_Depth.h"
#include "ViveSR_DepthMesh.h"
#include "ViveSR_RigidReconstruction.h"
#include "ViveSR_ControllerPose.h"
#include "ViveSR_Enums.h"

#define LOCTEXT_NAMESPACE "FSRWorksModule"


void FSRWorksModule::StartupModule()
{
	// Get the base directory of this plugin
	BaseDir = FPaths::ProjectPluginsDir() + "SRWorks";

#if PLATFORM_WINDOWS
	const FString SRWorksAPILibName = "SRWork_Client_API.dll";
#elif PLATFORM_MAC
    //LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/SRWorksLibrary/Mac/Release/libExampleLibrary.dylib"));
#endif // PLATFORM_WINDOWS

	LoadLibraryDLL(SRWorksAPILibName, SRWorksAPIDllHandle);
}

void FSRWorksModule::ShutdownModule()
{
	// Free the dll handle
	FPlatformProcess::FreeDllHandle(SRWorksAPIDllHandle);
	SRWorksAPIDllHandle = nullptr;
}

int FSRWorksModule::StartFramework()
{	
	SRWorksResult = ViveSR::SRWork::Initial(ViveSR::SRWork::PassThrough::MODULE_PASS_THROUGHT_TYPE, NULL);
	if (SRWorksResult != ViveSR::Error::WORK) { 
        UE_LOG(LogTemp, Error, TEXT("[ViveSR] ViveSR_Initial PassThrough failed. %d"), SRWorksResult);
        if (ViveSR::Error::EULA_NOT_ACCEPT == SRWorksResult) return SRWorksResult;
    }

    SRWorksResult = ViveSR::SRWork::Initial(ViveSR::SRWork::PassThrough4K::MODULE_PASS_THROUGHT_4K_TYPE, NULL);
    if (SRWorksResult != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] ViveSR_Initial PassThrough4K failed. %d"), SRWorksResult); }
	
    SRWorksResult = ViveSR::SRWork::Initial(ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE, NULL);
	if (SRWorksResult != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] ViveSR_Initial Depth failed. %d"), SRWorksResult);  }
	
    SRWorksResult = ViveSR::SRWork::Initial(ViveSR::SRWork::DepthMesh::MODULE_DEPTH_MESH_TYPE, NULL);
	if (SRWorksResult != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] ViveSR_Initial DepthMesh failed. %d"), SRWorksResult);  }
		
	SRWorksResult = ViveSR::SRWork::Initial(ViveSR::SRWork::RigidReconstruction::MODULE_RIGID_RECONSTRUCTION_TYPE, NULL);
	if (SRWorksResult != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] ViveSR_Initial Reconstruction failed. %d"), SRWorksResult);  }
	
    SRWorksResult = ViveSR::SRWork::Initial(ViveSR::SRWork::Controller::MODULE_CONTROLLER_POSE_TYPE, NULL);
    if (SRWorksResult != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] ViveSR_Initial Controller failed. %d"), SRWorksResult); }
	return SRWorksResult;
}

int FSRWorksModule::StopFramework()
{
	int result=0;

	result = ViveSR::SRWork::Release(ViveSR::SRWork::PassThrough::MODULE_PASS_THROUGHT_TYPE);
	if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] Stop PassThrough failed. %d"), result);  }
	
    result = ViveSR::SRWork::Release(ViveSR::SRWork::PassThrough4K::MODULE_PASS_THROUGHT_4K_TYPE);
    if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] Stop PassThrough4K failed. %d"), result); }

	result = ViveSR::SRWork::Release(ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE);
	if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] Stop Depth failed. %d"), result); }	

	result = ViveSR::SRWork::Release(ViveSR::SRWork::DepthMesh::MODULE_DEPTH_MESH_TYPE);
	if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] Stop DepthMesh failed. %d"), result);  }
	
	
	result = ViveSR::SRWork::Release(ViveSR::SRWork::RigidReconstruction::MODULE_RIGID_RECONSTRUCTION_TYPE);
	if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] Stop Reconstruction failed. %d"), result);  }
	
    result = ViveSR::SRWork::StopViveSR();
	return result;
}
int FSRWorksModule::UnLinkModule() {
    int result = 0;
    result = ViveSR::SRWork::PassThrough::TurnOffUndistortDataToDepth();
    if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] TurnOffUndistortDataToDepth  failed. %d"), result); }
    result = ViveSR::SRWork::UnlinkModule(ViveSR::SRWork::PassThrough::MODULE_PASS_THROUGHT_TYPE, ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE);
    if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] UnLink PassThrough an Depth failed. %d"), result); }
    result = ViveSR::SRWork::UnlinkModule(ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE, ViveSR::SRWork::DepthMesh::MODULE_DEPTH_MESH_TYPE);
    if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] UnLink Depth an DepthMesh failed. %d"), result); }
    result = ViveSR::SRWork::UnlinkModule(ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE, ViveSR::SRWork::RigidReconstruction::MODULE_RIGID_RECONSTRUCTION_TYPE);
    if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] UnLink Depth an Reconstruction failed. %d"), result); }
    return result;
}
int FSRWorksModule::StartModule(int moduletype) {
	int result = ViveSR::SRWork::StartModule(moduletype);
	if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] Start %d Module failed. %d"), moduletype,result); }
	return result;
}
int FSRWorksModule::StopModule(int moduletype) {
	int result = ViveSR::SRWork::Release(moduletype);
	if (result != ViveSR::Error::WORK) { UE_LOG(LogTemp, Error, TEXT("[ViveSR] Start Module failed. %d"), result);  }
	return result;
}
bool FSRWorksModule::LoadLibraryDLL(const FString& libName, void*& handle)
{
	FString LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/Win64"), *libName);
	handle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
	return handle != nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSRWorksModule, SRWorks)