// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

UENUM(BlueprintType)
enum class FrameworkStatus : uint8
{
	WORKING,
	STOP,
	ERROR_
};

UENUM(BlueprintType)
enum class DualCameraMode : uint8
{
	REAL,
	MIX
};

UENUM(BlueprintType)
enum class DualCameraIndex : uint8
{
	LEFT,
	RIGHT
};

UENUM(BlueprintType)
enum class CalibrationType : uint8
{
	RELATIVE,
	ABSOLUTE
};

UENUM(BlueprintType)
enum class CalibrationAxis : uint8
{
	X, Y, Z
};

UENUM(BlueprintType)
enum class DualCameraDisplayMode : uint8
{
	VIRTUAL,
	MIX
};

UENUM(BlueprintType)
enum class DualCameraStatus : uint8
{
	NOT_FOUND,
	IDLE,
	WORKING,
	ERROR
};


UENUM(BlueprintType)
enum class ReconstructionDisplayMode : uint8
{
	NONE = 0,
	FIELD_OF_VIEW = 1,
	ADAPTIVE_MESH = 2,
};

UENUM(BlueprintType)
enum class CameraQuality : uint8
{
    NONE                    = 0,
	BRIGHTNESS				= 100,
	CONTRAST				= 101,
	HUE						= 102,
	SATURATION				= 103,
	SHARPNESS				= 104,
	GAMMA					= 105,
	//COLOR_ENABLE          = 106,
	WHITE_BALANCE			= 107,
	BACKLIGHT_COMPENSATION	= 108,
	GAIN					= 109,
	//PAN                   = 110,
	//TILT                  = 111,
	//ROLL                  = 112,
	//ZOOM                  = 113,
	//EXPOSURE              = 114,
	//IRIS                  = 115,
	//FOCUS                 = 116,
};

UENUM(BlueprintType)
enum class UDepthCase : uint8
{
	DEFAULT,
	CLOSE_RANGE,
};

UENUM(BlueprintType)
enum class GridSize : uint8
{
    TWO = 0        UMETA(DisplayName = "2cm"),
    FOUR = 1       UMETA(DisplayName = "4cm"),
    EIGHT = 2      UMETA(DisplayName = "8cm"),
    SIXTEEN = 3    UMETA(DisplayName = "16cm"),
    THIRTYTWO = 4  UMETA(DisplayName = "32cm"),
    SIXTYFOUR = 5  UMETA(DisplayName = "64cm"),
};

UENUM(BlueprintType)
enum class SceneObjectType : uint8
{
    FLOOR = 0,
    WALL = 1,
    CEILING = 2,
    CHAIR = 3,
    TABLE = 4,
    BED = 5,
    MONITOR = 6,
    WINDOW = 7,
};

UENUM(BlueprintType)
enum class ControllerHandType : uint8
{
    NOHAND,
    LEFT_HAND,
    RITHT_HAND,
};

UENUM(BlueprintType)
enum class VRDevice : uint8 
{
    None,
    VIVE_PRO,
    COSMOS,
};

UENUM(BlueprintType)
enum class ModuleType : uint8
{
    PASSTHROUGH = 0,
    DEPTH,
    DEPTHMESH,
    RIGIDRECONSTRUCTION,
    PASSTHROUGH4K,
    CONTROLLER_POSE,
    MODULE_TYPE_MAX  UMETA(Hidden),
};

UENUM(BlueprintType)
enum class ModuleStatus : uint8
{
    MODULE_STATUS_ERROR,
    MODULE_STATUS_IDLE,
    MODULE_STATUS_WORKING,
    MODULE_STATUS_BLOCKED
};