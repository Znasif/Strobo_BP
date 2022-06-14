// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "SRWork_Client_API.h"
#include "ViveSR_Enums.h"
#include "ViveSR_UEnums.h"
#include "ViveSR_RigidReconstructionEnums.h"
#include "ViveSR_RigidReconstructionData.h"
/**
 * 
 */

class ViveSR_SceneUnderstanding
{
public:
	ViveSR_SceneUnderstanding();
	~ViveSR_SceneUnderstanding();
	static ViveSR_SceneUnderstanding* Instance();
	bool enabled_sceneunderstanding = false;
	bool enabled_refinement = true;
	bool is_exporting = false;
    int processing_stage;
    int processing_bar;
    int progress_bar;

	void ResetParameter();
	int EnableSceneUnderstanding(bool enable);
	void EnableSceneUnderstandingRefinement(bool enable);
	void StartExporting(FString filename);
	void GetSceneUnderstandingProgress(int &stage,  int &percentage);
	void UpdateSceneUnderstandingProgress();
	int GetSceneUnderstandingProgress();
	int GetSceneUnderstandingConfig(ViveSR::SRWork::RigidReconstruction::SceneUnderstandingConfig *config);
	int SetSceneUnderstandingConfig(ViveSR::SRWork::RigidReconstruction::SceneUnderstandingConfig config);
	void SetCustomSceneUnderstandingConfig(ViveSR::RigidReconstruction::SceneUnderstandingObjectType objectType, int objectMaxNum, bool isOn);
	void SetAllCustomSceneUnderstandingConfig(int objectMaxNum, bool isOn);

private:
	static ViveSR_SceneUnderstanding *Mgr;
	void ResetSceneUnderstandingProgress();
};
