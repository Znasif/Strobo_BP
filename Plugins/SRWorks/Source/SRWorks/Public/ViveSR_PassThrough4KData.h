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
namespace ViveSR {

    namespace SRWork {

        namespace PassThrough4K {
            /** @struct Pass Through4KData
            * A struct containing all data listed below.
            */
            struct PassThrough4KData
            {
                char* distorted_4k_frame_left;
                char* distorted_4k_frame_right;
                char* undistorted_4k_frame_left;
                char* undistorted_4k_frame_right;
                unsigned int output4k_frameSeq;
                unsigned int output4k_timeStp;
                float* output4k_pose_left;// [16];
                float* output4k_pose_right;// [16];
                int output4k_lux_left;
                int output4k_lux_right;
                int output4k_color_temperature_left;
                int output4k_color_temperature_right;
                char* camera4k_params;            // [sizeof(dualcamera::DeviceInfo)]
                char* d3d11_texture2d_shared_handle_left;   // [sizeof(void *)]
                char* d3d11_texture2d_shared_handle_right;  // [sizeof(void *)]
                unsigned int d3d11_texture2d_buffer_index;
            };
        }
    }
}