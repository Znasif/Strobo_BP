namespace ViveSR
{
	namespace PassThrough4K
	{
        /**
        An enum type for pass through 4K parameters.
        */
        enum Param
        {
			OUTPUT_DISTORTED_4K_WIDTH = 0,
			OUTPUT_DISTORTED_4K_HEIGHT,
			OUTPUT_DISTORTED_4K_CHANNEL,
			OUTPUT_UNDISTORTED_4K_WIDTH,
			OUTPUT_UNDISTORTED_4K_HEIGHT,
			OUTPUT_UNDISTORTED_4K_CHANNEL,
			OUTPUT_4K_OFFSET_HEAD_TO_CAMERA,		// float[6]  (x0,y0,z0,x1,y1,z1)
			CAMERA_4K_PARAMETERS,					// void *
			OFFSET_HEAD_TO_4K_CAMERA_x0,			// float[6]  (x0,y0,z0,x1,y1,z1)
			OFFSET_HEAD_TO_4K_CAMERA_y0,
			OFFSET_HEAD_TO_4K_CAMERA_z0,
			OFFSET_HEAD_TO_4K_CAMERA_x1,
			OFFSET_HEAD_TO_4K_CAMERA_y1,
			OFFSET_HEAD_TO_4K_CAMERA_z1,
			OUTPUT_4K_READY,						// bool
			D3D11_SHARED_HANDLE_4K_UNDISTORTED_LEFT,//int
			D3D11_SHARED_HANDLE_4K_UNDISTORTED_RIGHT,//int
            CAMERA_4K_MODE,//int
            CAMERA_4K_ZOOM,//int
            CAMERA_4K_FOV,
            UNDISTORT_GPU_TO_CPU_ENABLE
		};
        /**
        An enum type for camera parameters.
        */
        enum CAMERA_Param
        {
			CAMERA_4K_FCX_0 = 0,					   // DOUBLE
			CAMERA_4K_FCX_1,                           // DOUBLE
			CAMERA_4K_FCY_0,                           // DOUBLE
			CAMERA_4K_FCY_1,                           // DOUBLE
			CAMERA_4K_FLEN_0,                          // DOUBLE
			CAMERA_4K_FLEN_1,                          // DOUBLE
			CAMERA_4K_ROTATION_0,                      // DOUBLE
			CAMERA_4K_ROTATION_1,                      // DOUBLE
			CAMERA_4K_ROTATION_2,                      // DOUBLE
			CAMERA_4K_ROTATION_3,                      // DOUBLE
			CAMERA_4K_ROTATION_4,                      // DOUBLE
			CAMERA_4K_ROTATION_5,                      // DOUBLE
			CAMERA_4K_ROTATION_6,                      // DOUBLE
			CAMERA_4K_ROTATION_7,                      // DOUBLE
			CAMERA_4K_ROTATION_8,                      // DOUBLE
			CAMERA_4K_TRANSLATION_0,                   // DOUBLE
			CAMERA_4K_TRANSLATION_1,                   // DOUBLE
			CAMERA_4K_TRANSLATION_2,                   // DOUBLE
			UNDISTORTION_4K_CENTER_CX_L,               // DOUBLE
			UNDISTORTION_4K_CENTER_CY_L,               // DOUBLE
			UNDISTORTION_4K_CENTER_CX_R,               // DOUBLE
			UNDISTORTION_4K_CENTER_CY_R,               // DOUBLE
			CAMERA_4K_PARAMS_MAX
        };
        /**
        An enum type for VR initial type.
        */
		enum InitType {
			SCENE = 1,	   // Application will submit 3D frames
			BACKGROUND = 3 // Application should not start SteamVR if it's not already running, and should not
		};
        /**
        An enum type for pass through 4K output data structure.
        */
		enum OutputMask
		{
			DISTORTED_4K_FRAME_LEFT = 0,	// sizeof(char) * 1920 * 1920 * 4
			DISTORTED_4K_FRAME_RIGHT = 1,	// sizeof(char) * 1920 * 1920 * 4
			UNDISTORTED_4K_FRAME_LEFT = 2,	// sizeof(char) * 2464 * 2464 * 4 or 1920 * 1920* 4 
			UNDISTORTED_4K_FRAME_RIGHT = 3,	// sizeof(char) * 2464 * 2464 * 4 or 1920 * 1920* 4
			OUTPUT4K_FRAME_SEQ = 4,			// sizeof(unsigned int)
			OUTPUT4K_TIME_STP = 5,			// sizeof(unsigned int)
			OUTPUT4K_POSE_LEFT = 6,			// sizeof(float) * 16
			OUTPUT4K_POSE_RIGHT = 7,		// sizeof(float) * 16
			OUTPUT4K_LUX_LEFT = 8,			// sizeof(int)
			OUTPUT4K_LUX_RIGHT = 9,			// sizeof(int)
			OUTPUT4K_COLOR_TEMPERATURE_LEFT = 10,	// sizeof(int)
			OUTPUT4K_COLOR_TEMPERATURE_RIGHT = 11,	// sizeof(int)
			OUTPUT4K_CAMERA_PARAMS = 12,	// sizeof(dualcamera::DeviceInfo)
			D3D11_TEXTURE2D_SHARED_HANDLE_LEFT = 13,	// sizeof(void *)
			D3D11_TEXTURE2D_SHARED_HANDLE_RIGHT = 14,	// sizeof(void *)
			D3D11_TEXTURE2D_BUFFER_INDEX = 15,	// sizeof(unsigned int)
			MAX,
		};
	}
}
