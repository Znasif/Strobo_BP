#pragma once
namespace ViveSR
{
    namespace Controller
    {
        /** @enum OutputMask
        An enum type for see-through output data structure.
        */
        enum OutputMask
        {
            POSE_IS_VALID_LEFT,            // sizeof(unsigned int)
            POSE_IS_VALID_RIGHT,           // sizeof(unsigned int)
            POSE_POSITION_LEFT,            // sizeof(float)*3
            POSE_POSITION_RIGHT,           // sizeof(float)*3
            POSE_ROTATION_LEFT,            // sizeof(float)*4
            POSE_ROTATION_RIGHT,           // sizeof(float)*4
            MAX,
        };
    }
}
