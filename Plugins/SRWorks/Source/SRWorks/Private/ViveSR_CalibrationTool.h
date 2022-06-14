// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ViveSR_UEnums.h"
#include "HAL/FileManager.h"
#include "Runtime/XmlParser/Public/XmlParser.h"

class ViveSR_CalibrationTool
{
public:
	ViveSR_CalibrationTool();
	~ViveSR_CalibrationTool();

	static ViveSR_CalibrationTool* Instance();
	
	float AngleCalculation(FVector2D init_position, FVector2D current_position);
	void SaveParameter(FRotator calibrationResult);
	TArray<float> LoadParameter();
private:
	static ViveSR_CalibrationTool *Mgr;
};
