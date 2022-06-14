#pragma once

#ifdef SRWORK_MODULE_API_EXPORTS
#define SR_WORK __declspec(dllexport)
#else
#define SR_WORK __declspec(dllimport)
#endif

namespace ViveSR {
    namespace SRWork {

        extern "C" {
            /** Convert depth to pass through or pass through 4k depth.
            * @param[in] srcF: depth. pose_<>: pose with sizeof(float)x16. flen_depth: focal length in depth module.
            * @param[out] dstF: warp depth.
            * @return Indicates the resulting Vive.Plugin.SR.Error status of this method.
            */
            SR_WORK int DepthWarp_Warp(const float *srcF,
                float *dstF, float *pose_depth, float*pose_passthrough, float flen_depth);
        }
    }
}
