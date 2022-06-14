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
#include "ViveSR_DepthMeshData.h"

#ifdef SRWORK_MODULE_API_EXPORTS
#define SR_WORK __declspec(dllexport)
#else
#define SR_WORK __declspec(dllimport)
#endif


extern "C" {

    namespace ViveSR {

        namespace SRWork {

            namespace DepthMesh {

                const int MODULE_DEPTH_MESH_TYPE = ModuleTypeOfClient::DEPTHMESH;

                /** Gets the DepthMeshData from SRWorks engine.
                * @param[out] data ViveSR::SRWork::DepthMesh::DepthMeshData
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetDepthMeshData(DepthMeshData *data);
                
                /** Enable or disable depth mesh of module.
                * @param[in] value Is the boolean to set enable or disable depth mesh.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetDepthMeshIsEnable(bool value);

                /** Sets the depth parameter boolean of module.
                * @param[in] parameter_name ViveSR::SRWork::DepthMesh::Param
                * @param[in] param_value The parameter input value.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetDepthMeshParameterBool(int parameter_name, bool param_value);

                /** Sets the depth parameter integer of module.
                * @param[in] parameter_name ViveSR::SRWork::DepthMesh::Param
                * @param[in] param_value The parameter input value.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetDepthMeshParameterInt(int parameter_name, int param_value);

                /** Sets the depth parameter double of module.
                * @param[in] parameter_name ViveSR::SRWork::DepthMesh::Param
                * @param[in] param_value The parameter input value.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetDepthMeshParameterDouble(int parameter_name, double param_value);

                /** Get the depth parameter integer from module.
                * @param[in] parameter_name ViveSR::SRWork::DepthMesh::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetDepthMeshParameterInt(int parameter_name, int* param_value);

                /** Set the depth mesh maximum update interval of SRWorks engine.
                * @param[in] max_fps The maximum fps want to set.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetDepthMeshMaxFps(int max_fps);
            }
        }
    }
}