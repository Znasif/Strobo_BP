// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "SRWork_Client_API.h"
#include "ViveSR_Enums.h"
#include "ViveSR_UEnums.h"
#include "ViveSR_PassThrough.h"
#include "ViveSR_PassThroughEnums.h"
#include "ViveSR_PassThrough4K.h"
#include "ViveSR_PassThrough4KEnums.h"
#include "ViveSR_Depth.h"
#include "ViveSR_DepthEnums.h"
#include "RHICommandList.h"
#include "RHIContext.h"
#include "D3D11RHI.h"
#include "vector"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/AllowWindowsPlatformAtomics.h"
#include "d3d11.h"
#include "Windows/HideWindowsPlatformAtomics.h"

/**
 * 
 */
namespace ViveSR {
    namespace PassThrough {
        struct CameraSettings
        {
            int32_t Status;
            int32_t DefaultValue;
            int32_t Min;
            int32_t Max;
            int32_t Step;
            int32_t DefaultMode;
            int32_t Value;
            int32_t Mode;  // AUTO = 1, MANUAL = 2
        };
    }
}
class ViveSR_DualCameraImageCapture
{
public:
	ViveSR_DualCameraImageCapture();
	~ViveSR_DualCameraImageCapture();
	static ViveSR_DualCameraImageCapture* Instance();
	static void DestroyImageCapture();
	double* camera_param;
	int distorted_img_width = 612;
	int distorted_img_height = 460;
	int distorted_img_channel = 4;
	int undistorted_img_width = 1150;
	int undistorted_img_height = 750;
	int UndistortedImageChannel = 4;
	
	int depth_img_width = 640;
	int depth_img_height = 480;
	int depth_img_channel = 1;
	int depth_data_size = 4;
	
	double distorted_cx_left;
	double distorted_cy_left;
	double distorted_cx_right;
	double distorted_cy_right;
	double undistorted_cx_left;
	double undistorted_cy_left;
	double undistorted_cx_right;
	double undistorted_cy_right;
	double *camera_params = new double[22];
	FMatrix distorted_pose_left, distorted_pose_right;
	FMatrix undistorted_pose_left, undistorted_pose_right;
	FMatrix depth_pose;
	FVector GetDistortedLocalPosition(DualCameraIndex eye);
	FRotator GetDistortedLocalRotation(DualCameraIndex eye);
	FVector GetUndistortedLocalPosition(DualCameraIndex eye);
	FRotator GetUndistortedLocalRotation(DualCameraIndex eye);
	FVector GetDepthLocalPosition();
	FRotator GetDepthLocalRotation();
	float depth_confidence_threshold;
	float depth_denoise_guided_filter;
	float depth_denoise_median_filter;
	void Initial();
	void Release();
	void GetDistortedTexture(UTexture2D*& img_left, UTexture2D*& img_right, int& frame_idx, int& time_idx);
	void GetUndistortedTexture(UTexture2D*& img_left, UTexture2D*& img_right, int& frame_idx, int& time_idx);
	void GetDepthTexture(UTexture2D*& imageDepth, int& frame_idx, int& time_idx);

	bool UpdateDistortedImage();
    bool UpdateUndistortedImage();
	bool UpdateDepthImage();

	bool is_depth_processing = false;
	bool is_depth_refinement_enabled = false;
	bool is_depth_edge_enhance_enabled = false;
	ViveSR::Depth::DepthCase depth_case;

	int EnableDepthProcess(bool active);
	int EnableDepthRefinement(bool active);
    bool IsDepthRefinementActive();
	int EnableDepthEdgeEnhance(bool active);
	int SetDepthCase(ViveSR::Depth::DepthCase depthCase);
	int SetDepthConfidenceThreshold(float value);
	int SetDepthDenoiseGuidedFilter(int value);
	int SetDepthDenoiseMedianFilter(int value);

	int GetCameraSettings(ViveSR::PassThrough::CameraControlType item, ViveSR::PassThrough::CameraSettings *param_info);
	int SetCamera(ViveSR::PassThrough::CameraControlType item, ViveSR::PassThrough::CameraSettings *param_info);
    
	double focal_length_left;
	double focal_length_right;
	double base_line;	
	float head_to_camera_offset[6];

    bool img4K_ready = false;
    
    int get_PassThrough_result;

    bool is_gpu_path = false;
    void SetGPUMethod(bool is_gpu_path);

	float GetGamma();
    bool IsHorus();
    ViveSR::SRWork::PassThrough::PassThroughData *pass_through_data = nullptr;
    ViveSR::SRWork::PassThrough4K::PassThrough4KData *pass_through4K_data = nullptr;
private:
	static ViveSR_DualCameraImageCapture *Mgr;

	void GetParameters();
	void TextureRegionCleanUp(uint8* rawData, const FUpdateTextureRegion2D* region) {};
    bool GetD3D11ShaderResourceView(void* texture_shared_handle, void** shader_resource_view);

	TFunction<void(uint8*, const FUpdateTextureRegion2D*)> texCleanUpFP = [this](uint8* rawData, const FUpdateTextureRegion2D* region) { TextureRegionCleanUp(rawData, region); };

	UTexture2D* distorted_texture_left = nullptr;
	UTexture2D* distorted_texture_right = nullptr;
	UTexture2D* undistorted_texture_left = nullptr;
	UTexture2D* undistorted_texture_right = nullptr;
	UTexture2D* depth_texture = nullptr;
  

    std::vector<ID3D11Resource*>resource_from_handle_left;
    std::vector<ID3D11Resource*>resource_from_handle_right;
    
    ID3D11Device *native_device = nullptr;
    ID3D11DeviceContext* device_context =nullptr;

    ID3D11ShaderResourceView* srv_from_handle_left = nullptr;
    ID3D11ShaderResourceView* srv_from_handle_right = nullptr;


	FUpdateTextureRegion2D* distorted_texture_region = nullptr;
	FUpdateTextureRegion2D* undistorted_texture_region = nullptr;
	FUpdateTextureRegion2D* depth_texture_region = nullptr;

	
    
    std::vector<void*>srv_left;
    std::vector<void*>srv_right;
    
    TMap<int, int> idx_map;

    int vertex_idx = 0;

	ViveSR::SRWork::Depth::DepthData *depth_data = nullptr;
	uint8 *distorted_raw_bgra_left = nullptr, *distorted_raw_bgra_right = nullptr;
	uint8 *undistorted_raw_bgra_left = nullptr, *undistorted_raw_bgra_right = nullptr;
	uint8 *depth_raw = nullptr;

	FVector Position(FMatrix m);
};
