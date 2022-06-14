#pragma once
namespace ViveSR
{
    namespace DepthMesh
    {
        /**
        An enum type for DepthMesh parameters.
        */
        enum Param {
            OUTPUT_WIDTH,
            OUTPUT_HEIGHT,
            OUTPUT_CHAANEL_0,
            OUTPUT_CHAANEL_1,
            TYPE,
            FOCULENS,
            BASELINE,
            COLLIDER_QUALITY,
            MESH_NEAR_DISTANCE,
            MESH_FAR_DISTANCE,
            DENOISE_MEDIAN_FILTER,	// range : 1, 3, 5; (default: 3)
            CONFIDENCE_THRESHOLD,	// range : 0 ~ 5; (default: 0.05)
            DENOISE_GUIDED_FILTER,  // range : 1 ~ 7; (default: 3)
            DEPTH_USING_CASE,
        };

        /**
        An enum type for depth mesh type.
        */
        enum DepthCase
        {
            DEFAULT,
            CLOSE_RANGE,
        };

        /**
        An enum type for depth mesh input data structure.
        */
        enum InputMask
        {
            _LEFT_FRAME = 0,		        // sizeof(char) * 640 * 480 * 4
            _DEPTH_MAP = 1,		            // sizeof(float) * 640 * 480 * 1
            _FRAME_SEQ = 2,		            // sizeof(unsigned int)
            _TIME_STP = 3,		            // sizeof(unsigned int)
            _POSE = 4,		                // sizeof(float) * 16
            _LUX_LEFT = 5,		            // sizeof(int)
            _COLOR_TEMPERATURE_LEFT = 6,	// sizeof(int)
            _EXPOSURE_TIME_LEFT = 7,		// sizeof(int)
            _ANALOG_GAIN_LEFT = 8,		    // sizeof(int)
            _DIGITAL_GAIN_LEFT = 9,		    // sizeof(int)
            _CAMERA_PARAMS = 10,            // sizeof(dualcamera::DeviceInfo)
            INPUT_MASK_MAX
        };
        /**
        An enum type for depth mesh output data structure.
        */
        enum OutputMask
        {
            TIME_STP = 0,		            // sizeof(unsigned int)
            POSE = 1,		                // sizeof(float) * 16
            NUM_VERTICES = 2,		        // sizeof(unsigned int)
            BYTEPERVERT = 3,		        // sizeof(unsigned int)
            VERTICES = 4,		            // sizeof(float) * 640 * 480 * 3
            NUM_INDICES = 5,		        // sizeof(unsigned int)
            INDICES = 6,		            // sizeof(int) * 640 * 480 * 6
            LUX_LEFT = 7,		            // sizeof(int)
            COLOR_TEMPERATURE_LEFT = 8,		// sizeof(int)
            EXPOSURE_TIME_LEFT = 9,		    // sizeof(int)
            ANALOG_GAIN_LEFT = 10,		    // sizeof(int)
            DIGITAL_GAIN_LEFT = 11,		    // sizeof(int)
            CAMERA_PARAMS = 12,             // sizeof(dualcamera::DeviceInfo)
            OUTPUT_MASK_MAX
        };

        /**
        An enum type for control signal to change depth mesh module's behavior.
        */
        enum Cmd {
            EXTRACT_DEPTH_MESH = 100,
            ENABLE_SELECT_MESH_DISTANCE_RANGE,
            ENABLE_DEPTH_MESH_HOLE_FILLING,
        };
    }

}