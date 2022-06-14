// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_CalibrationTool.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Math/UnrealMathUtility.h"
#include <string>
#include <fstream>
#include <direct.h>

ViveSR_CalibrationTool* ViveSR_CalibrationTool::Mgr = nullptr;
ViveSR_CalibrationTool::ViveSR_CalibrationTool()
{

}
ViveSR_CalibrationTool::~ViveSR_CalibrationTool()
{
}
ViveSR_CalibrationTool* ViveSR_CalibrationTool::Instance()
{
	if (Mgr == nullptr)
	{
		Mgr = new ViveSR_CalibrationTool();
	}
	return Mgr;
}
float ViveSR_CalibrationTool::AngleCalculation(FVector2D init_position, FVector2D current_position) {
    float start_angle = 0.0f;
    float current_angle = 0.0f;
    float rotating_angle = 0.0f;
    FVector2D xAxis = FVector2D(1, 0);
	if (init_position.Y < 0) {
		start_angle = FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(init_position, xAxis) / init_position.Size()));
		current_angle = FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(current_position, xAxis) / current_position.Size()));
	}
	else {
		start_angle = 360-FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(init_position, xAxis) / init_position.Size()));
		current_angle = 360 - FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(current_position, xAxis) / current_position.Size()));
	}

	rotating_angle = current_angle - start_angle;
	if (rotating_angle>0 && rotating_angle > 180)
		rotating_angle = rotating_angle-360;
	if (rotating_angle < 0 && rotating_angle < -180)
		rotating_angle = 360 + rotating_angle;
	return rotating_angle;
}
void ViveSR_CalibrationTool::SaveParameter(FRotator calibrationResult) {
	FString app_path = FPlatformProcess::UserSettingsDir();
	app_path.RemoveFromEnd("Local/");
	FString full_path = app_path + "LocalLow/HTC Corporation/Calibration/";
	std::string file_path = std::string(TCHAR_TO_UTF8(*full_path));
	_mkdir(file_path.c_str());
	std::ofstream calibration_param(file_path + "calibration_param.txt");
	calibration_param << std::to_string(calibrationResult.Pitch) << std::endl;
	calibration_param << std::to_string(calibrationResult.Roll)<< std::endl;
	calibration_param << std::to_string(calibrationResult.Yaw) << std::endl;
	calibration_param.close();

}
TArray<float> ViveSR_CalibrationTool::LoadParameter() {
	TArray<float>Result;
	FString app_path = FPlatformProcess::UserSettingsDir();
	app_path.RemoveFromEnd("Local/");
	FString full_path = app_path + "LocalLow/HTC Corporation/Calibration/";
	std::string file_path = std::string(TCHAR_TO_UTF8(*full_path));
	std::ifstream read_file;
	read_file.open(file_path + "calibration_param.txt", std::ios::binary);
	std::string line;
	while (getline(read_file, line)) {
		Result.Push((float)atof(line.c_str()));
	}
	return Result;
}