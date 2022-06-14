///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//                           License Agreement
//                     For Vive Super Reality Library
//
// Copyright (c) 2017,		HTC Corporation
//
// All rights reserved. Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
///////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ViveSR_ModuleEnums.h"
#include "ViveSR_PassThroughData.h"

#ifdef SRWORK_MODULE_API_EXPORTS
#define SR_WORK __declspec(dllexport)
#else
#define SR_WORK __declspec(dllimport)
#endif


extern "C" {

    namespace ViveSR {

        namespace SRWork {

            namespace PassThrough {

                const int MODULE_PASS_THROUGHT_TYPE = ModuleTypeOfClient::PASS_THROUGH;

                /** Gets the PassThroughData from SRWorks engine.
                * @param[out] data ViveSR::SRWork::PassThrough::PassThroughData
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetPassThroughData(PassThroughData *data);

                /** Get the camera parameters from SRWorks engine.
                * @param[out] camera_params The camera parameter array. The array of ViveSR::SRWork::PassThrough::CAMERA_Param
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetCameraParams(double* camera_params);

                /** Get the pass through parameter boolean from module.
                * @param[in] parameter_name ViveSR::SRWork::PassThrough::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetPassThrougParameterBool(int parameter_name, bool* param_value);

                /** Get the pass through parameter integer from module.
                * @param[in] parameter_name ViveSR::SRWork::PassThrough::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetPassThrougParameterInt(int parameter_name, int* param_value);

                /** Get the pass through parameter float from module.
                * @param[in] parameter_name ViveSR::SRWork::PassThrough::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetPassThrougParameterFloat(int parameter_name, float *param_value);

                /** Get the pass through parameter double from module.
                * @param[in] parameter_name ViveSR::SRWork::PassThrough::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetPassThrougParameterDouble(int parameter_name, double* param_value);
                /**
                 * Get camera control status.
                 * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                 * @param[out] status The camera status pointer.
                 * @return Indicate the API's error code.
                 */
                SR_WORK int GetCameraStatus(int camera_control_type, int32_t* status);
                /**
                * Get camera control default value.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] default_value The camera default value pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int GetCameraDefaultValue(int camera_control_type, int32_t* default_value);
                /**
                * Get camera control min.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] min The camera min pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int GetCameraMin(int camera_control_type, int32_t* min);
                /**
                * Get camera control max.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] max The camera max pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int GetCameraMax(int camera_control_type, int32_t* max);
                /**
                * Get camera control step.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] step The camera step pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int GetCameraStep(int camera_control_type, int32_t* step);
                /**
                * Get camera control default mode.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] default_mode The camera default mode pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int GetCameraDefaultMode(int camera_control_type, int32_t* default_mode);
                /**
                * Get camera control value.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] value The camera value pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int GetCameraValue(int camera_control_type, int32_t* value);
                /**
                * Get camera control mode.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] mode The camera mode pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int GetCameraMode(int camera_control_type, int32_t* mode);
                /**
                * Set camera control status.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] status The camera status pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int SetCameraStatus(int camera_control_type, int32_t status);
                /**
                * Set camera control default value.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] default_value The camera default value pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int SetCameraDefaultValue(int camera_control_type, int32_t default_value);
                /**
                * Set camera control min.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] min The camera min pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int SetCameraMin(int camera_control_type, int32_t min);
                /**
                * Set camera control max.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] max The camera max pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int SetCameraMax(int camera_control_type, int32_t max);
                /**
                * Set camera control step.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] step The camera step pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int SetCameraStep(int camera_control_type, int32_t step);
                /**
                * Set camera control default mode.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] default_mode The camera default mode pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int SetCameraDefaultMode(int camera_control_type, int32_t default_mode);
                /**
                * Set camera control value.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] value The camera value pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int SetCameraValue(int camera_control_type, int32_t value);
                /**
                * Set camera control mode.
                * @param[in] camera_control_type Which camera control type would you want to get.The parameters reference from ViveSR::PassThrough::Param::CameraControlType.
                * @param[out] mode The camera mode pointer.
                * @return Indicate the API's error code.
                */
                SR_WORK int SetCameraMode(int camera_control_type, int32_t mode);

                /** Set the pass through maximum update interval of SRWorks engine.
                * @param[in] max_fps The maximum fps want to set.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetPassThroughMaxFps(int max_fps);

                /** Get the controller tracking pose from pass through module.
                * @param[out] pPoses The pointer of pose struct.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetControllerTrackingPose(TrackingPoseInfo* pPoses);

                SR_WORK int SetSkipVGAProcess(bool skip);

                SR_WORK int GetSkipVGAProcess(bool* skip);

                /** Turn on the GPU to CPU path of the pass through module undistortion data with the depth module processing.
                * @return Indicate the API's error code.
                */
                SR_WORK int TurnOnUndistortDataToDepth();

                /** Turn off the GPU to CPU path of the pass through module undistortion data with the depth module processing.
                * @return Indicate the API's error code.
                */
                SR_WORK int TurnOffUndistortDataToDepth();

                /** Turn on the getting distortion data method of the pass through module.
                * @return Indicate the API's error code.
                */
                SR_WORK int TurnOnDistortData();

                /** Turn off the getting distortion data method of the pass through module.
                * @return Indicate the API's error code.
                */
                SR_WORK int TurnOffDistortData();
            }
        }
    }
}