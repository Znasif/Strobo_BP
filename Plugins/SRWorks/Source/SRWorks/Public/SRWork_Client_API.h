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
#include "ViveSR_Enums.h"
#include "SRWork_module_Enums.h"
#ifdef SRWORK_MODULE_API_EXPORTS
#define SR_WORK __declspec(dllexport)
#else
#define SR_WORK __declspec(dllimport)
#endif

using RuntimeStatusCB = void(*)(int);

/** Vive Super Reality
*/
namespace ViveSR {
    namespace SRWork {

        extern "C" {
            /** Turn on the specific SRWorks engine.
            * @param[in] ModuleType What kind of engine should be turn on
            * @param[in] config
            * @return one of ViveSR::Error
            */
            SR_WORK int Initial(int ModuleType, void* config);

            /** Initial the specific SRWorks engine without start.
            * @param[in] ModuleType What kind of engine should be turn on
            * @param[in] config 
            * @return one of ViveSR::Error
            */
            SR_WORK int InitialWOStart(int ModuleType, void* config);

            /** Start the specific SRWorks engine.
            * @param[in] ModuleType What kind of engine should be turn on
            * @return one of ViveSR::Error
            */
            SR_WORK int StartModule(int ModuleType);

            /** Turn off and release the specific SRWorks engine.
            * @param[in] ModuleType What kind of engine should be turn off
            * @return one of ViveSR::Error
            */
            SR_WORK int Release(int ModuleType);

            /** Get the status of the specific SRWorks engine.
            * @param[in] ModuleType What kind of engine is interested in
            * @param[out] status The status of the specific engine
            * @return one of ViveSR::Error
            */
            SR_WORK int GetStatus(int ModuleType, ModuleStatus *status);

            /** Get the SRruntime version.
            * @param[out] version Represented by four integer
            * @return one of ViveSR::Error
            */
            SR_WORK int SR_WORK_GetVersion(char *& version);

            /** Link two modules.
            * @param[in] module_type_in The first module.
            * @param[in] module_type_out The second module.
            * @return one of ViveSR::Error
            */
            SR_WORK int LinkModule(int module_type_in, int module_type_out);

            /** Unlink two modules.
            * @param[in] module_type_in The first module.
            * @param[in] module_type_out The second module.
            * @return one of ViveSR::Error
            */
            SR_WORK int UnlinkModule(int module_type_in, int module_type_out);

            /** Stop ViveSR all modules.
            * @return one of ViveSR::Error
            */
            SR_WORK int StopViveSR();

            /** Stop ViveSR all modules.
            * @param[in] callback Callback function for runtime status changes.
            * @return one of ViveSR::Error
            */
            SR_WORK int SetRuntimeStatusCallback(RuntimeStatusCB callback);
        }
    }
}