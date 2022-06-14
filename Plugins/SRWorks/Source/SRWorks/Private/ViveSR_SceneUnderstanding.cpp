// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_SceneUnderstanding.h"
#include "SRWorks.h"
#include "SRWork_Client_API.h"
#include "ViveSR_PassThrough.h"
#include "Math/UnrealMathUtility.h"
#include "Runtime/Core/Public/HAL/FileManager.h"
#include "ViveSR_RigidReconstruction.h"
#include "ViveSR_RigidReconstructionEnums.h"
#include "ViveSR_URigidReconstruction.h"

ViveSR_SceneUnderstanding* ViveSR_SceneUnderstanding::Mgr = nullptr;
ViveSR_SceneUnderstanding::ViveSR_SceneUnderstanding()
{

}
ViveSR_SceneUnderstanding::~ViveSR_SceneUnderstanding()
{
}
ViveSR_SceneUnderstanding* ViveSR_SceneUnderstanding::Instance()
{
	if (Mgr == nullptr)
	{
		Mgr = new ViveSR_SceneUnderstanding();
	}
	return Mgr;
}
int ViveSR_SceneUnderstanding::EnableSceneUnderstanding(bool enable) {
	int result_Enable = ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool((int)ViveSR::RigidReconstruction::SCENE_UNDERSTANDING_ENABLE, enable);
	int result_MachineVision = ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool((int)ViveSR::RigidReconstruction::SCENE_UNDERSTANDING_MACHINE_VISION, enable);

	if (result_Enable == ViveSR::Error::WORK && result_MachineVision == ViveSR::Error::WORK){
		enabled_sceneunderstanding = enable;
		ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool((int)ViveSR::RigidReconstruction::SEMANTIC_FUSION_ALL_AFTER_SCANNING, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] [Scene Understanding] Activation/Deactivation failed "));
		return result_MachineVision;
	}

	if (enabled_sceneunderstanding)
	{
		if (result_Enable != ViveSR::Error::WORK)
			UE_LOG(LogTemp, Warning, TEXT("[ViveSR] [Scene Understanding] Progress listener failed to register"));
	}
    return result_MachineVision;
}

void ViveSR_SceneUnderstanding::EnableSceneUnderstandingRefinement(bool enable) {
	int result;

	result = ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool((int)ViveSR::RigidReconstruction::SCENE_UNDERSTANDING_REFINEMENT, enable);
	if (result == (int)ViveSR::Error::WORK)
	{
		enabled_refinement = enable;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] [Scene Understanding] Setting Refinement failed"));
}

void ViveSR_SceneUnderstanding::ResetSceneUnderstandingProgress()
{
	processing_bar = 0;
	processing_stage = 0;
}

void ViveSR_SceneUnderstanding::UpdateSceneUnderstandingProgress() {
	ViveSR::SRWork::RigidReconstruction::GetSceneUnderstandingProgress(&progress_bar);
	if (progress_bar == 100)
		is_exporting = false;
}
void ViveSR_SceneUnderstanding::GetSceneUnderstandingProgress(int &stage, int &percentage)
{
	stage = processing_stage;
	percentage = processing_bar;
}

int ViveSR_SceneUnderstanding::GetSceneUnderstandingProgress()
{
	return progress_bar;
}

void ViveSR_SceneUnderstanding::StartExporting(FString filename)
{
	processing_bar = 0;
	processing_stage = 0;
	ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Param::SCENE_UNDERSTANDING_ENABLE, true);

	// scene understanding is incompatible with these following functionalities
    ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Param::ENABLE_SECTOR_GROUPING, false);
    ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Param::ENABLE_FRUSTUM_CULLING, false);
	ViveSR::SRWork::RigidReconstruction::SetSceneUnderstandingOutputFileName((char*)StringCast<ANSICHAR>(*filename).Get(), filename.Len() + 1);
	
    is_exporting = true;
}
int ViveSR_SceneUnderstanding::GetSceneUnderstandingConfig(ViveSR::SRWork::RigidReconstruction::SceneUnderstandingConfig *config)
{ 
	int result = ViveSR::SRWork::RigidReconstruction::GetSceneUnderstandingConfig(config);
    if (result != ViveSR::Error::WORK) 
	    UE_LOG(LogTemp, Warning, TEXT("Vive_GetSceneUnderstandingConfig: %d"), result);
	return result;
}
int ViveSR_SceneUnderstanding::SetSceneUnderstandingConfig(ViveSR::SRWork::RigidReconstruction::SceneUnderstandingConfig config)
{
	int result =ViveSR::SRWork::RigidReconstruction::SetSceneUnderstandingConfig(config);
    if (result != ViveSR::Error::WORK)
	    UE_LOG(LogTemp, Warning, TEXT("Vive_SetSceneUnderstandingConfig: %d"), result);
	return result;
}
void ViveSR_SceneUnderstanding::SetCustomSceneUnderstandingConfig(ViveSR::RigidReconstruction::SceneUnderstandingObjectType objectType, int objectMaxNum, bool isOn)
{
	ViveSR::SRWork::RigidReconstruction::SceneUnderstandingConfig *config = new ViveSR::SRWork::RigidReconstruction::SceneUnderstandingConfig();
	int result = GetSceneUnderstandingConfig(config);
	switch (objectType)
	{
	case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::BED:
		if (isOn) config->nBedMaxInst = objectMaxNum;
		else config->nBedMaxInst = 0;
		break;
	case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::CEILING:
		if (isOn) config->nCeilingMaxInst = objectMaxNum;
		else config->nCeilingMaxInst = 0;
		break;
	case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::CHAIR:
		if (isOn) config->nChairMaxInst = objectMaxNum;
		else config->nChairMaxInst = 0;
		break;
	case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::FLOOR:
		if (isOn) config->nFloorMaxInst = objectMaxNum;
		else config->nFloorMaxInst = 0;
		break;
	case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::TABLE:
		if (isOn) config->nTableMaxInst = objectMaxNum;
		else config->nTableMaxInst = 0;
		break;
	case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::WALL:
		if (isOn) config->nWallMaxInst = objectMaxNum;
		else config->nWallMaxInst = 0;
		break;
	case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::WINDOW:
		if (isOn) config->nWindowMaxInst = objectMaxNum;
		else config->nWindowMaxInst = 0;
		break;
	case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::MONITOR:
		if (isOn) config->nMonitorMaxInst = objectMaxNum;
		else config->nMonitorMaxInst = 0;
		break;
    case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::FURNITURE:
        if (isOn) config->nFurnitureMaxInst = objectMaxNum;
        else config->nFurnitureMaxInst = 0;
        break;
    case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::DOOR:
        if (isOn) config->nDoorMaxInst = objectMaxNum;
        else config->nDoorMaxInst = 0;
        break;
    case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::PICTURE:
        if (isOn) config->nPictureMaxInst = objectMaxNum;
        else config->nPictureMaxInst = 0;
        break;
    case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::PERSON:
        if (isOn) config->nPersonMaxInst = objectMaxNum;
        else config->nPersonMaxInst = 0;
        break;
    case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::LIGHT:
        if (isOn) config->nLightMaxInst = objectMaxNum;
        else config->nLightMaxInst = 0;
        break;
    case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::PLANT:
        if (isOn) config->nPlantMaxInst = objectMaxNum;
        else config->nPlantMaxInst = 0;
        break;
    case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::CURTAIN:
        if (isOn) config->nCurtainMaxInst = objectMaxNum;
        else config->nCurtainMaxInst = 0;
        break;
    case ViveSR::RigidReconstruction::SceneUnderstandingObjectType::PILLOW:
        if (isOn) config->nPillowMaxInst = objectMaxNum;
        else config->nPillowMaxInst = 0;
        break;
	}
	 result = SetSceneUnderstandingConfig(*config);
	 delete config;

}
void ViveSR_SceneUnderstanding::SetAllCustomSceneUnderstandingConfig(int objectMaxNum, bool isOn)
{
	for (int i = 0; i <= (int)ViveSR::RigidReconstruction::SceneUnderstandingObjectType::PILLOW; ++i)
	{
		SetCustomSceneUnderstandingConfig((ViveSR::RigidReconstruction::SceneUnderstandingObjectType)i, objectMaxNum, isOn);
	}
}
void ViveSR_SceneUnderstanding::ResetParameter()
{
	enabled_sceneunderstanding = false;
	enabled_refinement = true;
	is_exporting = false;
	processing_bar = 0;
}