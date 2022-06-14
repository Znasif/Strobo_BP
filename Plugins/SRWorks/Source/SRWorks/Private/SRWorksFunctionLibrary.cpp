// Fill out your copyright notice in the Description page of Project Settings.

#include "SRWorksFunctionLibrary.h"
#include "SRWorks.h"
#include "SRWork_Client_API.h"
#include "ViveSR_UEnums.h"
#include "ViveSR_PassThrough.h"
#include "ViveSR_PassThrough4K.h"
#include "ViveSR_Depth.h"
#include "ViveSR_DepthMesh.h"
#include "ViveSR_RigidReconstruction.h"
#include "ViveSR_DualCameraImageCapture.h"
#include "ViveSR_DualCameraDepthExtra.h"
#include "ViveSR_URigidReconstruction.h"
#include "ViveSR_SceneUnderstanding.h"
#include "ViveSR_SemanticObject.h"
#include "ViveSR_CalibrationTool.h"
#include "Misc/Paths.h"
#include "ViveSR_ModuleEnums.h"

#pragma region Framework
FString USRWorksFunctionLibrary::GetSRWorksVersion()
{
	char* version;
	int result = ViveSR::SRWork::SR_WORK_GetVersion(version);
	return version;
}
VRDevice USRWorksFunctionLibrary::GetDeviceType() {
    int system_device = 0;
    ViveSR::SRWork::PassThrough::GetPassThrougParameterInt(ViveSR::PassThrough::Param::DEVICE_SYSTEM_TYPE, &system_device);
    return (VRDevice)system_device;
}
bool USRWorksFunctionLibrary::IsHorus() {
    return ViveSR_DualCameraImageCapture::Instance()->IsHorus();
}
int USRWorksFunctionLibrary::StartModule(ModuleType type) {
    int result = FSRWorksModule::Get().StartModule((ModuleTypeOfClient)type);
    return result;
}
int USRWorksFunctionLibrary::StopModule(ModuleType type) {
    int result = FSRWorksModule::Get().StopModule((ModuleTypeOfClient)type);
    return result;
}
ModuleStatus USRWorksFunctionLibrary::GetStatus(ModuleType type) {
    ViveSR::SRWork::ModuleStatus module_status = ViveSR::SRWork::ModuleStatus::MODULE_STATUS_ERROR;
    int result = ViveSR::SRWork::GetStatus((ModuleTypeOfClient)type, &module_status);
    return (ModuleStatus)module_status;    
}
#pragma endregion

#pragma region DualCamera
bool USRWorksFunctionLibrary::GetDepthProcessing()
{
	return ViveSR_DualCameraImageCapture::Instance()->is_depth_processing;
}
int USRWorksFunctionLibrary::EnableDepthProcess(bool active)
{
	return ViveSR_DualCameraImageCapture::Instance()->EnableDepthProcess(active);
}
bool USRWorksFunctionLibrary::GetDepthRefinement()
{
	return ViveSR_DualCameraImageCapture::Instance()->is_depth_refinement_enabled;
}
int USRWorksFunctionLibrary::EnableDepthRefinement(bool active)
{
	return ViveSR_DualCameraImageCapture::Instance()->EnableDepthRefinement(active);
}
bool USRWorksFunctionLibrary::GetDepthEdgeEnhance()
{
	return ViveSR_DualCameraImageCapture::Instance()->IsDepthRefinementActive();
}
int USRWorksFunctionLibrary::EnableDepthEdgeEnhance(bool active)
{
	return ViveSR_DualCameraImageCapture::Instance()->EnableDepthEdgeEnhance(active);
}
UDepthCase USRWorksFunctionLibrary::GetDepthCase()
{
	return (UDepthCase)ViveSR_DualCameraImageCapture::Instance()->depth_case;
}
int USRWorksFunctionLibrary::SetDepthCase(UDepthCase depth_case)
{
	return ViveSR_DualCameraImageCapture::Instance()->SetDepthCase((ViveSR::Depth::DepthCase)depth_case);
}
float USRWorksFunctionLibrary::GetDepthConfidenceThreshold()
{
	return ViveSR_DualCameraImageCapture::Instance()->depth_confidence_threshold;
}
int USRWorksFunctionLibrary::SetDepthConfidenceThreshold(float value)
{
	return ViveSR_DualCameraImageCapture::Instance()->SetDepthConfidenceThreshold(value);
}
int USRWorksFunctionLibrary::GetDepthDenoiseG()
{
	return ViveSR_DualCameraImageCapture::Instance()->depth_denoise_guided_filter;
}
int USRWorksFunctionLibrary::SetDepthDenoiseG(int value)
{
	return ViveSR_DualCameraImageCapture::Instance()->SetDepthDenoiseGuidedFilter(value);
}
int USRWorksFunctionLibrary::GetDepthDenoiseM()
{
	return ViveSR_DualCameraImageCapture::Instance()->depth_denoise_median_filter;
}
int USRWorksFunctionLibrary::SetDepthDenoiseM(int value)
{
	return ViveSR_DualCameraImageCapture::Instance()->SetDepthDenoiseMedianFilter(value);
}

int USRWorksFunctionLibrary::GetCameraSettings(CameraQuality item, FParamInfo &param_info)
{
	ViveSR::PassThrough::CameraSettings *param = (ViveSR::PassThrough::CameraSettings *)malloc(sizeof(ViveSR::PassThrough::CameraSettings));
	int result = ViveSR_DualCameraImageCapture::Instance()->GetCameraSettings((ViveSR::PassThrough::CameraControlType)item, param);
	if (result != ViveSR::Error::WORK) {
		free(param);
		return result;
	}
    param_info.Status = param->Status;
	param_info.DefaultValue = param->DefaultValue;
	param_info.Min = param->Min;
	param_info.Max = param->Max;
	param_info.Step = param->Step;
	param_info.DefaultMode = param->DefaultMode;
	param_info.Value = param->Value;
	param_info.Mode = param->Mode;

	free(param);
	return result;
}
int USRWorksFunctionLibrary::SetCamera(CameraQuality item, FParamInfo param_info)
{
    ViveSR::PassThrough::CameraSettings *param = (ViveSR::PassThrough::CameraSettings *)malloc(sizeof(ViveSR::PassThrough::CameraSettings));
	param->Status = param_info.Status;
	param->DefaultValue = param_info.DefaultValue;
	param->Min = param_info.Min;
	param->Max = param_info.Max;
	param->Step = param_info.Step;
	param->DefaultMode = param_info.DefaultMode;
	param->Value = param_info.Value;
	param->Mode = param_info.Mode;
	int result = ViveSR_DualCameraImageCapture::Instance()->SetCamera((ViveSR::PassThrough::CameraControlType)item, param);
	free(param);
	return result;
}
#pragma endregion

#pragma region RigidReconstruction
int USRWorksFunctionLibrary::GetReconstructionResult()
{
    return ViveSR_URigidReconstruction::Instance()->get_data_result;
}
int USRWorksFunctionLibrary::EnableReconstructionProcess(bool active)
{
    return ViveSR_URigidReconstruction::Instance()->EnableReconstructionProcess(active);
}
void USRWorksFunctionLibrary::StartScanning()
{
	ViveSR_URigidReconstruction::Instance()->StartScanning();
}
void USRWorksFunctionLibrary::StopScanning()
{
	ViveSR_URigidReconstruction::Instance()->StopScanning();
}
int USRWorksFunctionLibrary::ResetReconstructionEngine() {
    return ViveSR_URigidReconstruction::Instance()->ResetReconstructionEngine();
}
bool USRWorksFunctionLibrary::GetIsScanning()
{
	return ViveSR_URigidReconstruction::Instance()->is_scanning;
}
bool USRWorksFunctionLibrary::GetIsExporting()
{
	return ViveSR_URigidReconstruction::Instance()->is_exporting;
}
void USRWorksFunctionLibrary::ExportReconstructionInfo(FString filename)
{
	ViveSR_URigidReconstruction::Instance()->StartExporting(filename);
}
int USRWorksFunctionLibrary::GetExportProgress()
{
	return ViveSR_URigidReconstruction::Instance()->GetExportProgress();
}
#pragma endregion

#pragma region SceneUnderstanding

void USRWorksFunctionLibrary::ExportSceneUnderstandingInfo(FString filename)
{
	ViveSR_SceneUnderstanding::Instance()->StartExporting(filename);
}
int USRWorksFunctionLibrary::GetSceneUnderstandingExportProgress()
{
	return ViveSR_SceneUnderstanding::Instance()->GetSceneUnderstandingProgress();
}
#pragma endregion

#pragma region SematicObject
int USRWorksFunctionLibrary::CheckChairNum() {
	return ViveSR_SemanticObject::Instance()->CheckChairNum();
}
TArray<FVector> USRWorksFunctionLibrary::GetChairLocation() {
	TArray<FVector> Location;
	ViveSR_SemanticObject::Instance()->GetChairLocation(Location);
	return Location;
}

TArray<FVector> USRWorksFunctionLibrary::GetChairForward() {
	TArray<FVector>Forward;
	ViveSR_SemanticObject::Instance()->GetChairForward(Forward);
	return Forward;
}
#pragma endregion
#pragma region CalibrationTool
float USRWorksFunctionLibrary::AngleCalculation(FVector2D positionIni, FVector2D positionCurrent) {
	return ViveSR_CalibrationTool::Instance()->AngleCalculation(positionIni, positionCurrent);
}
void USRWorksFunctionLibrary::SaveParameter(FRotator calibrationResult) {
	ViveSR_CalibrationTool::Instance()->SaveParameter(calibrationResult);
}
TArray<float> USRWorksFunctionLibrary::LoadParameter() {
	return ViveSR_CalibrationTool::Instance()->LoadParameter();
}
#pragma endregion

#pragma region SetFPS
int USRWorksFunctionLibrary::SetPassThroughMaxFps(int fps) {
    return ViveSR::SRWork::PassThrough::SetPassThroughMaxFps(fps);
}

int USRWorksFunctionLibrary::SetDepthMaxFps(int fps) {
    return ViveSR::SRWork::Depth::SetDepthMaxFps(fps);
}

int USRWorksFunctionLibrary::SetDepthMeshMaxFps(int fps) {
    return ViveSR::SRWork::DepthMesh::SetDepthMeshMaxFps(fps);
}

int USRWorksFunctionLibrary::SetReconstructionMaxFps(int fps) {
    return ViveSR::SRWork::RigidReconstruction::SetReconstructionMaxFps(fps);
}

#pragma endregion
bool USRWorksFunctionLibrary::FileExists(const FString &InPath)
{
	return FPaths::FileExists(InPath);
}
FString USRWorksFunctionLibrary::ProjectUserDir()
{
	return FPaths::ProjectUserDir();
}
FString USRWorksFunctionLibrary::GetSaveFileRootPath()
{
	FString app_path = FPlatformProcess::UserSettingsDir();
	app_path.RemoveFromEnd("Local/");
	app_path = app_path + "LocalLow/HTC Corporation/SR_Reconstruction_Output/";
	return app_path;
}