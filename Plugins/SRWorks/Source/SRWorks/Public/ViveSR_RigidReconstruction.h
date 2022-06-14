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
#include "ViveSR_RigidReconstructionData.h"

#ifdef SRWORK_MODULE_API_EXPORTS
#define SR_WORK __declspec(dllexport)
#else
#define SR_WORK __declspec(dllimport)
#endif

using ExportProgessCB = void(*)(int, int);

extern "C" {

    namespace ViveSR {

        namespace SRWork {

            namespace RigidReconstruction {

                const int MODULE_RIGID_RECONSTRUCTION_TYPE = ModuleTypeOfClient::RIGID_RECONSTRUCTION;

                /** Gets the RigidReconstructionData from SRWorks engine.
                * @param[out] DepthData ViveSR::SRWork::RigidReconstruction::RigidReconstructionData
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetRigidReconstructionData(RigidReconstructionData *data);

                /** Sets the rigidReconstruction parameter boolean of module.
                * @param[in] parameter_name ViveSR::SRWork::RigidReconstruction::Param
                * @param[in] param_value The parameter input value.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetReconstructionParameterBool(int parameter_name, bool param_value);
                
                /** Sets the rigidReconstruction parameter integer of module.
                * @param[in] parameter_name ViveSR::SRWork::RigidReconstruction::Param
                * @param[in] param_value The parameter input value.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetReconstructionParameterInt(int parameter_name, int param_value);
                
                /** Sets the rigidReconstruction parameter float of module.
                * @param[in] parameter_name ViveSR::SRWork::RigidReconstruction::Param
                * @param[in] param_value The parameter input value.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetReconstructionParameterFloat(int parameter_name, float param_value);
                
                /** Sets the rigidReconstruction parameter double of module.
                * @param[in] parameter_name ViveSR::SRWork::RigidReconstruction::Param
                * @param[in] param_value The parameter input value.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetReconstructionParameterDouble(int parameter_name, double param_value);
                
                /** Get the rigidReconstruction parameter boolean from module.
                * @param[in] parameter_name ViveSR::SRWork::RigidReconstruction::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetReconstructionParameterBool(int parameter_name, bool* param_value);

                /** Get the rigidReconstruction parameter integer from module.
                * @param[in] parameter_name ViveSR::SRWork::RigidReconstruction::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetReconstructionParameterInt(int parameter_name, int* param_value);

                /** Get the rigidReconstruction parameter float from module.
                * @param[in] parameter_name ViveSR::SRWork::RigidReconstruction::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetReconstructionParameterFloat(int parameter_name, float* param_value);

                /** Get the rigidReconstruction parameter double from module.
                * @param[in] parameter_name ViveSR::SRWork::RigidReconstruction::Param
                * @param[out] param_value The parameter output value pointer.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetReconstructionParameterDouble(int parameter_name, double* param_value);

                /** Set the scene understanding config which set maximum counts in one label object.
                * @param[in] SceneUnderstandingConfig ViveSR::SRWork::RigidReconstruction::SceneUnderstandingConfigId
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetSceneUnderstandingConfig(SceneUnderstandingConfig value);

                /** Get  the scene understanding config which set maximum counts in one label object.
                * @param[out] SceneUnderstandingConfig ViveSR::SRWork::RigidReconstruction::SceneUnderstandingConfigId
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetSceneUnderstandingConfig(SceneUnderstandingConfig* value);

                /** Set the reconstruction output filename and export mesh.
                * @param[in] parameter_name The array of file name.
                * @param[in] size The file name length.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetReconstructionOutputFileName(char* parameter_name, int size);
                
                /** Set the scene understanding output filename and export scene understanding object.
                * @param[in] parameter_name The array of file name.
                * @param[in] size The file name length.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetSceneUnderstandingOutputFileName(char* parameter_name, int size);
                
                /** Set the reconstruction module output path.
                * @param[in] parameter_name The folder path.
                * @param[in] size The folder path length.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetReconstructionOutputFolder(char* parameter_name, int size);

                /** Get the reconstruction mesh export progress.
                * @param[out] value The progress between 0 to 100.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetExportMeshProgress(int* value);

                /** Get the scene understanding export progress.
                * @param[out] value The progress between 0 to 100.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetSceneUnderstandingProgress(int* value);

                /** Set the reconstruction maximum update interval of SRWorks engine.
                * @param[in] max_fps The maximum fps want to set.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetReconstructionMaxFps(int max_fps);

                /** Reset Reconstruction Module and clean scanned data.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int ResetReconstructionModule();

                /** Set Reconstruction Geometry Size, this API only can be used before scanning.
                * @param[in] Geometry Size in meter
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetReconstructionGeometrySize(float geometrySize);

                /** Set Reconstruction Color Size, this API only can be used before scanning.
                * @param[in] Color Size in meter
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int SetReconstructionColorSize(float colorSize);

                /** Set Reconstruction Geometry Size.
                * @param[out] Geometry Size in meter
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetReconstructionGeometrySize(float* geometrySize);

                /** Get Reconstruction Color Size.
                * @param[out] Color Size in meter
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int GetReconstructionColorSize(float* colorSize);

                /** Terminate Exporting inside Reconstruction Module.
                * @return Indicates the resulting ViveSR::Error status of this method.
                */
                SR_WORK int TerminateExporting();
            }
        }
    }
}