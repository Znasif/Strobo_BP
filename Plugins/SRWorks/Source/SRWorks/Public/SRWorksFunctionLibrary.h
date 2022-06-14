// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViveSR_UEnums.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SRWorksFunctionLibrary.generated.h"

/**
 * @brief SRWorks function library for blueprints
 */

USTRUCT(BlueprintType)
struct FParamInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "ParamInfo Struct") int32 Status;
    UPROPERTY(BlueprintReadWrite, Category = "ParamInfo Struct") int32 DefaultValue;
    UPROPERTY(BlueprintReadWrite, Category = "ParamInfo Struct") int32 Min;
    UPROPERTY(BlueprintReadWrite, Category = "ParamInfo Struct") int32 Max;
    UPROPERTY(BlueprintReadWrite, Category = "ParamInfo Struct") int32 Step;
    UPROPERTY(BlueprintReadWrite, Category = "ParamInfo Struct") int32 DefaultMode;
    UPROPERTY(BlueprintReadWrite, Category = "ParamInfo Struct") int32 Value;
    UPROPERTY(BlueprintReadWrite, Category = "ParamInfo Struct") int32 Mode;

    FParamInfo() :Status(0), DefaultValue(0), Min(0), Max(0), Step(0), DefaultMode(0), Value(0), Mode(0) {
    }
};

UCLASS()
class SRWORKS_API USRWorksFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/** Get the version of  SRWorks. 
    * @return version of SRWorks
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|Framework")
	static FString GetSRWorksVersion();

    /** Get Device Type.
    * @return one of VRDevice
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR|Framework")
    static VRDevice GetDeviceType();

    /** Get whether Device is Horus.
    * @return true if the Device is Horus.
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR|Framework")
    static bool IsHorus();

    /** Start specific module
    * @param ModuleType spcific which module
    * @return one of ViveSR::Error
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR|Framework")
    static int StartModule(ModuleType type);

	/** Stop specific module
    * @param ModuleType spcific which module
    * @return one of ViveSR::Error
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|Framework")
	static int StopModule(ModuleType type);

    /** Get the status of the specific SRWorks engine.
    * @param[in] ModuleType What kind of engine is interested in
    * @return The status of the specific engine
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR|Framework")
    static ModuleStatus GetStatus(ModuleType type);

	/** Get whether the depth process is active. 
    * @return true if the depth process is active.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static bool GetDepthProcessing();

	/** Eanble or disable the depth process and return the error code of result.
    * @param active set enable or disable.
    * @return one of ViveSR::Error
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static int EnableDepthProcess(bool active);

	/** Get whether depth refinement is active.
    * @return true if the depth refinement is active.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static bool GetDepthRefinement();

	/** Eanble or disable the depth refinement.
    * @param active set enable or disable.
    * @return one of ViveSR::Error
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static int EnableDepthRefinement(bool active);

    /** Get whether depth edge enhance is active.
    * @return true if the depth edge enhance is active.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static bool GetDepthEdgeEnhance();

    /** Eanble or disable the depth edge enhance.
    * @param active set enable or disable.
    * @return one of ViveSR::Error
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static int EnableDepthEdgeEnhance(bool active);

    /** Get the depth case.
    * @return one of UDepthCase
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static UDepthCase GetDepthCase();

    /** Set the depth case.
    * @param depthCase spcific which case
    * @return one of ViveSR::Error
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static int SetDepthCase(UDepthCase depthCase);

    /** Get the depth confidence threshold.
    * @return depth confidence threshold
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static float GetDepthConfidenceThreshold();

    /** Set the depth confidence threshold.
    * @param value set the value of depth confidence threshold
    * @return one of ViveSR::Error
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static int SetDepthConfidenceThreshold(float value);

    /** Get the Depth Denoise Guided Filter.
    * @return Depth Denoise Guided Filter.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static int GetDepthDenoiseG();

    /** Set the Depth Denoise Guided Filter.
    * @param value set the value of Depth Denoise Guided Filter
    * @return one of ViveSR::Error
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static int SetDepthDenoiseG(int value);

    /** Get the Depth Denoise Median Filter.
    * @return Depth Denoise Median Filter.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static int GetDepthDenoiseM();

    /** Set the Depth Denoise Median Filter.
    * @param value set the value of Depth Denoise Median Filter.
    * @return one of ViveSR::Error
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|Depth")
	static int SetDepthDenoiseM(int value);

	/** Get camera quality information
    * @param item specific camera quality item
    * @param 
    is information of camera quality
    * @return one of ViveSR::Error
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera")
	static int GetCameraSettings(CameraQuality item, FParamInfo &param_info);

    /** Set camera quality information
    * @param item specific camera quality item
    * @param paramInfo is information of camera quality
    * @return one of ViveSR::Error
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera")
	static int SetCamera(CameraQuality item, FParamInfo param_info);

    /** Get the return value of GetRigidReconstructionData. */
    UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction")
    static int GetReconstructionResult();

    /** Eanble or disable the reconstruction process and return the error code of result.
    * @param active set enable or disable.
    * @return one of ViveSR::Error
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction")
    static int EnableReconstructionProcess(bool active);

	/** Start to scan the real scene. */
	UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction")
	static void StartScanning();

	/** Stop scaning the real scene. */
	UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction")
	static void StopScanning();

    /** Reset Reconstruction Engine. */
    UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction")
    static int ResetReconstructionEngine();


	/** Get whether the rigid reconstruction module is scaning.
    * @return true if the rigid reconstruction module is scaning.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction")
	static bool GetIsScanning();

    /** Get whether models are being exported.
    * @return true if there are models are being exported.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction")
	static bool GetIsExporting();

	/** Start to export current scene and create an obj file
    * @param filename set the name of obj file
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction")
	static void ExportReconstructionInfo(FString filename);

	/** Get the percentage of current export progress.
    * @return the percentage of current export progress.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction")
	static int GetExportProgress();

	/** Start to export sceneunderstanding and create an obj and xml file
    * @param filename set the output folder name of sceneunderstanding object
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|SceneUnderstanding")
	static void ExportSceneUnderstandingInfo(FString filename);

	/** Get the percentage of current export progress of sceneunderstanding.
    * @return the percentage of current export progress of sceneunderstanding.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|SceneUnderstanding")
	static int GetSceneUnderstandingExportProgress();

	/** Check the number of chair for Chair Segmentation
    * @return the number of chair
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|SematicObject")
	static int CheckChairNum();

	/** Get the Location of chairs for Chair Segmentation
    * @return the position of chairs from xml file
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|SematicObject")
	static TArray<FVector> GetChairLocation();

	/** Get the Forward of chairs for Chair Segmentation
    * @return the forward of chairs from xml file
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|SematicObject")
	static TArray<FVector> GetChairForward();

	/** Get the angle of two vector for Calibration
    * @param positionIni is initial position at touchpad
    * @param positionCurrent is current position at touchpad
    * @return the angle between two vector in degree
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|CalibrationTool")
	static float AngleCalculation(FVector2D positionIni, FVector2D positionCurrent);

	/** save the parameter of Rotator for Calibration
    * @param calibrationResult is result of calibration
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|CalibrationTool")
	static void SaveParameter(FRotator calibrationResult);

	/** Load the parameter of Rotator for Calibration
    * @return the calibration result
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|CalibrationTool")
	static TArray<float> LoadParameter();

    /** Set PassThroughFPS
    * @param fps set the fps of PassThrough
    * @return one of ViveSR::Error
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR|SetFPS")
    static int SetPassThroughMaxFps(int fps);

    /** Set Depth FPS 
    * @param fps set the fps of depth
    * @return one of ViveSR::Error
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR|SetFPS")
    static int SetDepthMaxFps(int fps);

    /** Set DepthMesh FPS
    * @param fps set the fps of depth mesh
    * @return one of ViveSR::Error
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR|SetFPS")
    static int SetDepthMeshMaxFps(int fps);

    /** Set Reconstruction FPS
    * @param fps set the fps of Reconstruction
    * @return one of ViveSR::Error
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR|SetFPS")
    static int SetReconstructionMaxFps(int fps);

    /** Check whether the file exists
    * @return true if this file was found
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|Utility|File")
	static bool FileExists(const FString &InPath);

    /** Get the root directory for user-specific game files.
    * @return root directory for user-specific game files.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|Utility|File")
	static FString ProjectUserDir();

	/**Get the root directory for saving files.
    * @return root directory for saving files.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR|Utility|File")
	static FString GetSaveFileRootPath();
};