#pragma once
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
// Copyright (c) 2019,		HTC Corporation
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
#include <stdint.h>
#include "ViveSR_RigidReconstructionEnums.h" 

namespace ViveSR {

    namespace SRWork {

        namespace RigidReconstruction {
            /** @struct RigidReconstructionData
            * A struct containing rigidReconstruction data listed below.
            */
            struct RigidReconstructionData
            {
                unsigned int frame_seq;	        // sizeof(unsigned int)
                float* posemtx44;               // sizeof(float) * 16
                int num_vertices;	            // sizeof(int)
                int bytepervert;	            // sizeof(int)
                float* vertices;	            // sizeof(float) * 8 * 2500000
                int num_indices;	            // sizeof(int)
                int* indices;	                // sizeof(int) * 2500000
                int cldtype;	                // sizeof(int)
                unsigned int collidernum;	    // sizeof(unsigned int)
                unsigned int *cld_num_verts;	// sizeof(unsigned int) * 200
                unsigned int *cld_numidx;	    // sizeof(unsigned int) * 200
                float *cld_vertices;	        // sizeof(float) * 3 * 50000
                int *cld_indices;	            // sizeof(int) * 100000
                int sector_num;                 // sizeof(int)
                int *sector_id_list;            // sizeof(int) * 1000000
                int *sector_vert_num;           // sizeof(int) * 1000000
                int *sector_idx_num;            // sizeof(int) * 1000000
                int model_chunk_num;            // sizeof(int)
                int model_chunk_idx;            // sizeof(int)
            };
            /** @struct SceneUnderstandingConfig
            * A struct containing scene understanding config listed below.
            */
            struct SceneUnderstandingConfig {
                union
                {
                    struct
                    {
                        int32_t nFloorMaxInst;
                        int32_t nWallMaxInst;
                        int32_t nCeilingMaxInst;
                        int32_t nChairMaxInst;
                        int32_t nTableMaxInst;
                        int32_t nBedMaxInst;
                        int32_t nMonitorMaxInst;
                        int32_t nWindowMaxInst;
                        int32_t nFurnitureMaxInst;
                        int32_t nDoorMaxInst;
                        int32_t nPictureMaxInst;
                        int32_t nPersonMaxInst;
                        int32_t nLightMaxInst;
                        int32_t nPlantMaxInst;
                        int32_t nCurtainMaxInst;
                        int32_t nPillowMaxInst;
                    };
                    int32_t nMaxDetectInstance[ViveSR::RigidReconstruction::SceneUnderstandingObjectType::TOTAL];
                };
            };
        }
    }
}