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
#include "ViveSR_PassThrough4KData.h"

#ifdef SRWORK_MODULE_4K_API_EXPORTS
#define SR_WORK __declspec(dllexport)
#else
#define SR_WORK __declspec(dllimport)
#endif


extern "C" {

    namespace ViveSR {

        namespace SRWork {

            namespace PassThrough4K {

                const int MODULE_PASS_THROUGHT_4K_TYPE = ModuleTypeOfClient::PASS_THROUGH_4K;

                /** Gets the PassThrough4KData from SRWorks engine.
                * @param[out] data ViveSR::SRWork::PassThrough4K::PassThrough4KData
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetPassThrough4KData(PassThrough4KData *data);

                /** Get the camera parameters from SRWorks engine.
                * @param[out] camera_params The camera parameter array. The array of ViveSR::SRWork::PassThrough4K::CAMERA_Param
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int Get4KCameraParams(double* camera_params);

                /** Get the Pass through 4K parameter boolean from module.
                * @param[in] parameter_name ViveSR::SRWork::PassThrough4K::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetPassThroug4KParameterBool(int parameter_name, bool* param_value);

                /** Get the Pass through 4K parameter integer from module.
                * @param[in] parameter_name ViveSR::SRWork::PassThrough4K::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetPassThroug4KParameterInt(int parameter_name, int* param_value);

                /** Get the Pass through 4K parameter float from module.
                * @param[in] parameter_name ViveSR::SRWork::PassThrough4K::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetPassThroug4KParameterFloat(int parameter_name, float *param_value);

                /** Get the Pass through 4K parameter double from module.
                * @param[in] parameter_name ViveSR::SRWork::PassThrough4K::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetPassThroug4KParameterDouble(int parameter_name, double* param_value);

                /** Set the Pass through 4K maximum update interval of SRWorks engine.
                * @param[in] max_fps The maximum fps want to set.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetPassThrough4KMaxFps(int max_fps);
                /** Set the Pass through 4K parameter as bool type.
                * @param[in] type The type of parameter.
                * @param[in] value The value of parameter.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetPassThrough4kParameterBool(int type, bool value);
                /** Set the Pass through 4K parameter as int type.
                * There are 3 types of parameter this function supported in the file: "ViveSR_PassThrough4KEnums.h"
                * They are CAMERA_4K_MODE,CAMERA_4K_ZOOM and CAMERA_4K_FOV.
                * The camera mode can be set to the Default mode(0) or the Quality mode(1) or the Performance mode (2).
                * The camera zoom in mode can be set to the Default mode(0) or the Zoom-in mode(1)
                * The range of FOV is from 0 to 140.
                * @param[in] type The type of parameter.
                * @param[in] value The value of parameter.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetPassThrough4kParameter(int type, int value);
            }
        }
    }
}