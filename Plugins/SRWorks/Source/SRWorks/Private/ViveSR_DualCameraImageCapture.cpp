// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_DualCameraImageCapture.h"
#include "SRWorks.h"
#include "ViveSR_Enums.h"
#include "SRWork_Client_API.h"
#include "Math/UnrealMathUtility.h"
#include <algorithm>

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

ViveSR_DualCameraImageCapture* ViveSR_DualCameraImageCapture::Mgr = nullptr;
ViveSR_DualCameraImageCapture::ViveSR_DualCameraImageCapture()
{
	
}

ViveSR_DualCameraImageCapture::~ViveSR_DualCameraImageCapture()
{
}




void ViveSR_DualCameraImageCapture::Initial()
{
	GetParameters();
    
    // D3D11Device
    native_device = (ID3D11Device*)GDynamicRHI->RHIGetNativeDevice();
    native_device->GetImmediateContext(&device_context);
    

	// Distorted
    bool distorted_enable = false;
    if (distorted_enable) {
        if (!is_gpu_path) {
            distorted_raw_bgra_left = new uint8[distorted_img_width * distorted_img_height * distorted_img_channel];
            distorted_raw_bgra_right = new uint8[distorted_img_width * distorted_img_height * distorted_img_channel];
            std::fill_n(distorted_raw_bgra_left, distorted_img_width * distorted_img_height * distorted_img_channel, 0);
            std::fill_n(distorted_raw_bgra_right, distorted_img_width * distorted_img_height * distorted_img_channel, 0);
            distorted_texture_region = new FUpdateTextureRegion2D(0, 0, 0, 0, distorted_img_width, distorted_img_height);
        }

        distorted_texture_left = img4K_ready ? UTexture2D::CreateTransient(distorted_img_width, distorted_img_height, EPixelFormat::PF_B8G8R8A8) :
            UTexture2D::CreateTransient(distorted_img_width, distorted_img_height, EPixelFormat::PF_R8G8B8A8);
        distorted_texture_right = img4K_ready ? UTexture2D::CreateTransient(distorted_img_width, distorted_img_height, EPixelFormat::PF_B8G8R8A8) :
            UTexture2D::CreateTransient(distorted_img_width, distorted_img_height, EPixelFormat::PF_R8G8B8A8);
    
        distorted_texture_left->AddToRoot();
        distorted_texture_right->AddToRoot();
        distorted_texture_left->UpdateResource();
        distorted_texture_right->UpdateResource();
    }
	// Undistorted	
    if (!is_gpu_path) {
        undistorted_raw_bgra_left = new uint8[undistorted_img_width * undistorted_img_height * UndistortedImageChannel];
        undistorted_raw_bgra_right = new uint8[undistorted_img_width * undistorted_img_height * UndistortedImageChannel];
        std::fill_n(undistorted_raw_bgra_left, undistorted_img_width * undistorted_img_height * UndistortedImageChannel, 0);
        std::fill_n(undistorted_raw_bgra_right, undistorted_img_width * undistorted_img_height * UndistortedImageChannel, 0);
        undistorted_texture_region = new FUpdateTextureRegion2D(0, 0, 0, 0, undistorted_img_width, undistorted_img_height);
    }

    undistorted_texture_left = UTexture2D::CreateTransient(undistorted_img_width, undistorted_img_height, EPixelFormat::PF_R8G8B8A8);
    undistorted_texture_right = UTexture2D::CreateTransient(undistorted_img_width, undistorted_img_height, EPixelFormat::PF_R8G8B8A8);

	undistorted_texture_left->AddToRoot();
	undistorted_texture_right->AddToRoot();
	undistorted_texture_left->UpdateResource();
	undistorted_texture_right->UpdateResource();
	//Depth
	depth_raw = new uint8[depth_img_width * depth_img_height * depth_img_channel * sizeof(float)];
	depth_texture = UTexture2D::CreateTransient(depth_img_width, depth_img_height, EPixelFormat::PF_R32_FLOAT);
	depth_texture->AddToRoot();
	depth_texture->UpdateResource();
	depth_texture_region = new FUpdateTextureRegion2D(0, 0, 0, 0, depth_img_width, depth_img_height);
	// PassThrough
    if (!img4K_ready) {
        pass_through_data = new ViveSR::SRWork::PassThrough::PassThroughData();
        pass_through_data->distorted_frame_left = new char[distorted_img_width * distorted_img_height * distorted_img_channel];
        pass_through_data->distorted_frame_right = new char[distorted_img_width * distorted_img_height * distorted_img_channel];
        pass_through_data->undistorted_frame_left = new char[undistorted_img_width * undistorted_img_height * UndistortedImageChannel];
        pass_through_data->undistorted_frame_right = new char[undistorted_img_width * undistorted_img_height * UndistortedImageChannel];
        pass_through_data->pose_left = new float[16];
        pass_through_data->pose_right = new float[16];
        pass_through_data->camera_params = new char[1032];
        pass_through_data->d3d11_texture2d_shared_handle_left = new char[sizeof(HANDLE)];
        pass_through_data->d3d11_texture2d_shared_handle_right = new char[sizeof(HANDLE)];
    }
    else {
        pass_through4K_data = new ViveSR::SRWork::PassThrough4K::PassThrough4KData();
       // pass_through4K_data->distorted_4k_frame_left = new char[distorted_img_width * distorted_img_height * distorted_img_channel];
       // pass_through4K_data->distorted_4k_frame_right = new char[distorted_img_width * distorted_img_height * distorted_img_channel];
        pass_through4K_data->undistorted_4k_frame_left = new char[undistorted_img_width * undistorted_img_height * UndistortedImageChannel];
        pass_through4K_data->undistorted_4k_frame_right = new char[undistorted_img_width * undistorted_img_height * UndistortedImageChannel];
        pass_through4K_data->output4k_pose_left = new float[16];
        pass_through4K_data->output4k_pose_right = new float[16];
        pass_through4K_data->camera4k_params = new char[1032];
        pass_through4K_data->d3d11_texture2d_shared_handle_left = new char[sizeof(HANDLE)];
        pass_through4K_data->d3d11_texture2d_shared_handle_right = new char[sizeof(HANDLE)];
    }
	// Depth
	depth_data = new ViveSR::SRWork::Depth::DepthData();
	depth_data->left_frame = new char[640 * 480 * 4];
	depth_data->depth_map = new float[640 * 480 * 1];
	depth_data->pose = new float[16];
	depth_data->camera_params = new char[1032];
}
void ViveSR_DualCameraImageCapture::Release() {
	is_depth_processing = false;
	is_depth_refinement_enabled = false;
	is_depth_edge_enhance_enabled = false;
    
    srv_left.clear();
    srv_right.clear();
    idx_map.Empty();

    if (distorted_raw_bgra_left) {
        delete[] distorted_raw_bgra_left;
        distorted_raw_bgra_left = nullptr;
    }
    if (distorted_raw_bgra_right) {
        delete[] distorted_raw_bgra_right;
        distorted_raw_bgra_right = nullptr;
    }
    if (distorted_texture_region) {
        FMemory::Free(distorted_texture_region);
        distorted_texture_region = nullptr;
    }
    if (undistorted_raw_bgra_left) {
        delete[] undistorted_raw_bgra_left;
        undistorted_raw_bgra_left = nullptr;
    }
    if (undistorted_raw_bgra_right) {
        delete[] undistorted_raw_bgra_right;
        undistorted_raw_bgra_right = nullptr;
    }
    if (undistorted_texture_region) {
        FMemory::Free(undistorted_texture_region);
        undistorted_texture_region = nullptr;
    }
    if (depth_raw) {
        delete[] depth_raw;
        depth_raw = nullptr;
    }
    if (depth_texture_region) {
        FMemory::Free(depth_texture_region);
        depth_texture_region = nullptr;
    }
	// Device
    if (native_device) {
        native_device = nullptr;
    }
    if (device_context) {
        device_context = nullptr;
    }
	// PassThrough
	if (pass_through_data) {
        if (pass_through_data->distorted_frame_left){ 
            delete[] pass_through_data->distorted_frame_left; pass_through_data->distorted_frame_left=nullptr; 
        }
        if (pass_through_data->distorted_frame_right) {
            delete[] pass_through_data->distorted_frame_right; pass_through_data->distorted_frame_right = nullptr;
        }
        if (pass_through_data->undistorted_frame_left) {
            delete[] pass_through_data->undistorted_frame_left; pass_through_data->undistorted_frame_left = nullptr;
        }
        if (pass_through_data->undistorted_frame_right) {
            delete[] pass_through_data->undistorted_frame_right; pass_through_data->undistorted_frame_right = nullptr;
        }
        if (pass_through_data->d3d11_texture2d_shared_handle_left) {
            delete[] pass_through_data->d3d11_texture2d_shared_handle_left; pass_through_data->d3d11_texture2d_shared_handle_left = nullptr;
        }
        if (pass_through_data->d3d11_texture2d_shared_handle_right) {
            delete[] pass_through_data->d3d11_texture2d_shared_handle_right; pass_through_data->d3d11_texture2d_shared_handle_right = nullptr;
        }
        if (pass_through_data->pose_left) {
            delete[] pass_through_data->pose_left; pass_through_data->pose_left = nullptr;
        }
        if (pass_through_data->pose_right) {
            delete[] pass_through_data->pose_right; pass_through_data->pose_right = nullptr;
        }
        if (pass_through_data->camera_params) {
            delete[] pass_through_data->camera_params; pass_through_data->camera_params = nullptr;
        }
	}
    // PassThrough4K
    if (pass_through4K_data) {
        if (pass_through4K_data->undistorted_4k_frame_left) {
            delete[] pass_through4K_data->undistorted_4k_frame_left; pass_through4K_data->undistorted_4k_frame_left = nullptr;
        }
        if (pass_through4K_data->undistorted_4k_frame_right) {
            delete[] pass_through4K_data->undistorted_4k_frame_right; pass_through4K_data->undistorted_4k_frame_right = nullptr;
        }
        /*
        if (pass_through4K_data->distorted_4k_frame_left) {
            delete[] pass_through4K_data->distorted_4k_frame_left; pass_through4K_data->distorted_4k_frame_left = nullptr;
        }
        if (pass_through4K_data->distorted_4k_frame_right) {
            delete[] pass_through4K_data->distorted_4k_frame_right; pass_through4K_data->distorted_4k_frame_right = nullptr;
        }
        */
        if (pass_through4K_data->output4k_pose_left) {
            delete[] pass_through4K_data->output4k_pose_left; pass_through4K_data->output4k_pose_left = nullptr;
        }
        if (pass_through4K_data->output4k_pose_right) {
            delete[] pass_through4K_data->output4k_pose_right; pass_through4K_data->output4k_pose_right = nullptr;
        }
        if (pass_through4K_data->camera4k_params) {
            delete[] pass_through4K_data->camera4k_params; pass_through4K_data->camera4k_params = nullptr;
        }
        if (pass_through4K_data->d3d11_texture2d_shared_handle_left) {
            delete[] pass_through4K_data->d3d11_texture2d_shared_handle_left; pass_through4K_data->d3d11_texture2d_shared_handle_left = nullptr;
        }
        if (pass_through4K_data->d3d11_texture2d_shared_handle_right) {
            delete[] pass_through4K_data->d3d11_texture2d_shared_handle_right; pass_through4K_data->d3d11_texture2d_shared_handle_right = nullptr;
        }
    }
	// Depth
	if (depth_data) {
        if (depth_data->left_frame) {
            delete[] depth_data->left_frame; depth_data->left_frame = nullptr;
        }
        if (depth_data->depth_map) {
            delete[] depth_data->depth_map; depth_data->depth_map = nullptr;
        }
        if (depth_data->pose) {
            delete[] depth_data->pose; depth_data->pose = nullptr;
        }
        if (depth_data->camera_params) {
            delete[] depth_data->camera_params; depth_data->camera_params = nullptr;
        }
	}
    if (nullptr != pass_through_data) { delete[] pass_through_data; pass_through_data = nullptr; }
    if (nullptr != pass_through4K_data) { delete[] pass_through4K_data; pass_through4K_data = nullptr; }
    if (nullptr != depth_data) { delete[] depth_data; depth_data = nullptr; }
}

void ViveSR_DualCameraImageCapture::GetParameters()
{
    ViveSR::SRWork::PassThrough::GetPassThrougParameterBool(ViveSR::PassThrough::Param::OUTPUT_4K_READY, &img4K_ready);

    if (!img4K_ready) {
        ViveSR::SRWork::PassThrough::GetCameraParams(camera_params);

        ViveSR::SRWork::PassThrough::GetPassThrougParameterInt(ViveSR::PassThrough::Param::OUTPUT_DISTORTED_WIDTH, &distorted_img_width);
        ViveSR::SRWork::PassThrough::GetPassThrougParameterInt(ViveSR::PassThrough::Param::OUTPUT_DISTORTED_HEIGHT, &distorted_img_height);
        ViveSR::SRWork::PassThrough::GetPassThrougParameterInt(ViveSR::PassThrough::Param::OUTPUT_DISTORTED_CHANNEL, &distorted_img_channel);

        ViveSR::SRWork::PassThrough::GetPassThrougParameterInt(ViveSR::PassThrough::Param::OUTPUT_UNDISTORTED_WIDTH, &undistorted_img_width);
        ViveSR::SRWork::PassThrough::GetPassThrougParameterInt(ViveSR::PassThrough::Param::OUTPUT_UNDISTORTED_HEIGHT, &undistorted_img_height);
        ViveSR::SRWork::PassThrough::GetPassThrougParameterInt(ViveSR::PassThrough::Param::OUTPUT_UNDISTORTED_CHANNEL, &UndistortedImageChannel);

        int undistortionMapSize = 0;
        ViveSR::SRWork::PassThrough::GetPassThrougParameterInt(ViveSR::PassThrough::Param::UNDISTORTION_MAP_SIZE, &undistortionMapSize);

        //Get offset head to camera from hardware design.
        ViveSR::SRWork::PassThrough::GetPassThrougParameterFloat(ViveSR::PassThrough::Param::OFFSET_HEAD_TO_CAMERA_x0, &head_to_camera_offset[0]);
        ViveSR::SRWork::PassThrough::GetPassThrougParameterFloat(ViveSR::PassThrough::Param::OFFSET_HEAD_TO_CAMERA_y0, &head_to_camera_offset[1]);
        ViveSR::SRWork::PassThrough::GetPassThrougParameterFloat(ViveSR::PassThrough::Param::OFFSET_HEAD_TO_CAMERA_z0, &head_to_camera_offset[2]);
        ViveSR::SRWork::PassThrough::GetPassThrougParameterFloat(ViveSR::PassThrough::Param::OFFSET_HEAD_TO_CAMERA_x1, &head_to_camera_offset[3]);
        ViveSR::SRWork::PassThrough::GetPassThrougParameterFloat(ViveSR::PassThrough::Param::OFFSET_HEAD_TO_CAMERA_y1, &head_to_camera_offset[4]);
        ViveSR::SRWork::PassThrough::GetPassThrougParameterFloat(ViveSR::PassThrough::Param::OFFSET_HEAD_TO_CAMERA_z1, &head_to_camera_offset[5]);
    }
    else { 
        ViveSR::SRWork::PassThrough::SetSkipVGAProcess(true);
        ViveSR::SRWork::PassThrough4K::Get4KCameraParams(camera_params);
        
        ViveSR::SRWork::PassThrough4K::GetPassThroug4KParameterInt(ViveSR::PassThrough4K::Param::OUTPUT_UNDISTORTED_4K_WIDTH, &undistorted_img_width);
        ViveSR::SRWork::PassThrough4K::GetPassThroug4KParameterInt(ViveSR::PassThrough4K::Param::OUTPUT_UNDISTORTED_4K_HEIGHT, &undistorted_img_height);
        ViveSR::SRWork::PassThrough4K::GetPassThroug4KParameterInt(ViveSR::PassThrough4K::Param::OUTPUT_DISTORTED_4K_CHANNEL, &UndistortedImageChannel);

        //Get offset head to camera from hardware design.
        ViveSR::SRWork::PassThrough4K::GetPassThroug4KParameterFloat(ViveSR::PassThrough4K::Param::OFFSET_HEAD_TO_4K_CAMERA_x0, &head_to_camera_offset[0]);
        ViveSR::SRWork::PassThrough4K::GetPassThroug4KParameterFloat(ViveSR::PassThrough4K::Param::OFFSET_HEAD_TO_4K_CAMERA_y0, &head_to_camera_offset[1]);
        ViveSR::SRWork::PassThrough4K::GetPassThroug4KParameterFloat(ViveSR::PassThrough4K::Param::OFFSET_HEAD_TO_4K_CAMERA_z0, &head_to_camera_offset[2]);
        ViveSR::SRWork::PassThrough4K::GetPassThroug4KParameterFloat(ViveSR::PassThrough4K::Param::OFFSET_HEAD_TO_4K_CAMERA_x1, &head_to_camera_offset[3]);
        ViveSR::SRWork::PassThrough4K::GetPassThroug4KParameterFloat(ViveSR::PassThrough4K::Param::OFFSET_HEAD_TO_4K_CAMERA_y1, &head_to_camera_offset[4]);
        ViveSR::SRWork::PassThrough4K::GetPassThroug4KParameterFloat(ViveSR::PassThrough4K::Param::OFFSET_HEAD_TO_4K_CAMERA_z1, &head_to_camera_offset[5]);
    }
	distorted_cx_left = camera_params[0];
	distorted_cy_left = camera_params[2];
	distorted_cx_right = camera_params[1];
	distorted_cy_right = camera_params[3];
	focal_length_left = camera_params[4];
	focal_length_right = camera_params[5];
	
    undistorted_cx_left = camera_params[18];
    undistorted_cy_left = camera_params[19];
    undistorted_cx_right = camera_params[20];
    undistorted_cy_right = camera_params[21];

	ViveSR::SRWork::Depth::GetDepthParameterInt(ViveSR::Depth::Param::OUTPUT_WIDTH, & depth_img_width);
	ViveSR::SRWork::Depth::GetDepthParameterInt(ViveSR::Depth::Param::OUTPUT_HEIGHT, &depth_img_height);
	ViveSR::SRWork::Depth::GetDepthParameterInt(ViveSR::Depth::Param::OUTPUT_CHAANEL_1, &depth_img_channel);
	ViveSR::SRWork::Depth::GetDepthParameterDouble(ViveSR::Depth::Param::BASELINE, &base_line);
}

ViveSR_DualCameraImageCapture* ViveSR_DualCameraImageCapture::Instance()
{
	if (Mgr == nullptr)
	{
		Mgr = new ViveSR_DualCameraImageCapture();
	}
	return Mgr;
}

void ViveSR_DualCameraImageCapture::DestroyImageCapture()
{
	if (Mgr != nullptr)
	{
		delete Mgr;
		Mgr = nullptr;
	}
}

void ViveSR_DualCameraImageCapture::GetDistortedTexture(UTexture2D*& img_left, UTexture2D*& img_right, int& frame_idx, int& time_idx)
{
    if (!img4K_ready) {
        if (!is_gpu_path) {
            distorted_texture_left->UpdateTextureRegions(0, 1, distorted_texture_region, static_cast<uint32>(distorted_img_width * sizeof(uint8) * 4), sizeof(uint8) * 4, distorted_raw_bgra_left, texCleanUpFP);
            distorted_texture_right->UpdateTextureRegions(0, 1, distorted_texture_region, static_cast<uint32>(distorted_img_width * sizeof(uint8) * 4), sizeof(uint8) * 4, distorted_raw_bgra_right, texCleanUpFP);
        }
        else {
            if (get_PassThrough_result == 0) {
                FlushRenderingCommands();
                device_context->CopyResource((ID3D11Texture2D*)distorted_texture_right->Resource->TextureRHI->GetTexture2D()->GetNativeResource(), (ID3D11Texture2D*)resource_from_handle_right[idx_map[pass_through_data->d3d11_texture2d_buffer_index]]);
                device_context->CopyResource((ID3D11Texture2D*)distorted_texture_left->Resource->TextureRHI->GetTexture2D()->GetNativeResource(), (ID3D11Texture2D*)resource_from_handle_left[idx_map[pass_through_data->d3d11_texture2d_buffer_index]]);
            }
        }
        frame_idx = pass_through_data->frameSeq;
        time_idx = pass_through_data->timeStp;
    }
    else {
        if (!is_gpu_path) {
            distorted_texture_left->UpdateTextureRegions(0, 1, distorted_texture_region, static_cast<uint32>(distorted_img_width * sizeof(uint8) * 4), sizeof(uint8) * 4, distorted_raw_bgra_left, texCleanUpFP);
            distorted_texture_right->UpdateTextureRegions(0, 1, distorted_texture_region, static_cast<uint32>(distorted_img_width * sizeof(uint8) * 4), sizeof(uint8) * 4, distorted_raw_bgra_right, texCleanUpFP);
        }
        else {
            if (get_PassThrough_result == 0) {
                FlushRenderingCommands();
                device_context->CopyResource((ID3D11Texture2D*)distorted_texture_left->Resource->TextureRHI->GetTexture2D()->GetNativeResource(), (ID3D11Texture2D*)resource_from_handle_left[idx_map[pass_through4K_data->d3d11_texture2d_buffer_index]]);
                device_context->CopyResource((ID3D11Texture2D*)distorted_texture_right->Resource->TextureRHI->GetTexture2D()->GetNativeResource(), (ID3D11Texture2D*)resource_from_handle_right[idx_map[pass_through4K_data->d3d11_texture2d_buffer_index]]);
            }
        }
        frame_idx = pass_through4K_data->output4k_frameSeq;
        time_idx = pass_through4K_data->output4k_timeStp;
    }

	img_left = distorted_texture_left;
	img_right = distorted_texture_right;
}

void ViveSR_DualCameraImageCapture::GetUndistortedTexture(UTexture2D*& img_left, UTexture2D*& img_right, int& frame_idx, int& time_idx)
{
    if (!img4K_ready) {       
        if (!is_gpu_path) {
            undistorted_texture_left->UpdateTextureRegions(0, 1, undistorted_texture_region, static_cast<uint32>(undistorted_img_width * sizeof(uint8) * 4), sizeof(uint8) * 4, undistorted_raw_bgra_left, texCleanUpFP);
            undistorted_texture_right->UpdateTextureRegions(0, 1, undistorted_texture_region, static_cast<uint32>(undistorted_img_width * sizeof(uint8) * 4), sizeof(uint8) * 4, undistorted_raw_bgra_right, texCleanUpFP);
        }        
        else {
            if (get_PassThrough_result == 0) {
               FlushRenderingCommands();
               device_context->CopyResource((ID3D11Texture2D*)undistorted_texture_right->Resource->TextureRHI->GetTexture2D()->GetNativeResource(), (ID3D11Texture2D*)resource_from_handle_right[idx_map[pass_through_data->d3d11_texture2d_buffer_index]]);
               device_context->CopyResource((ID3D11Texture2D*)undistorted_texture_left->Resource->TextureRHI->GetTexture2D()->GetNativeResource(), (ID3D11Texture2D*)resource_from_handle_left[idx_map[pass_through_data->d3d11_texture2d_buffer_index]]);
            }
        }
        frame_idx = pass_through_data->frameSeq;
        time_idx = pass_through_data->timeStp;    
    }
    else {
        if (!is_gpu_path) {
            undistorted_texture_left->UpdateTextureRegions(0, 1, undistorted_texture_region, static_cast<uint32>(undistorted_img_width * sizeof(uint8) * 4), sizeof(uint8) * 4, undistorted_raw_bgra_left, texCleanUpFP);
            undistorted_texture_right->UpdateTextureRegions(0, 1, undistorted_texture_region, static_cast<uint32>(undistorted_img_width * sizeof(uint8) * 4), sizeof(uint8) * 4, undistorted_raw_bgra_right, texCleanUpFP);
        }
        else {
            if (get_PassThrough_result == 0) {
                FlushRenderingCommands();
                device_context->CopyResource((ID3D11Texture2D*)undistorted_texture_left->Resource->TextureRHI->GetTexture2D()->GetNativeResource(), (ID3D11Texture2D*)resource_from_handle_left[idx_map[pass_through4K_data->d3d11_texture2d_buffer_index]]);
                device_context->CopyResource((ID3D11Texture2D*)undistorted_texture_right->Resource->TextureRHI->GetTexture2D()->GetNativeResource(), (ID3D11Texture2D*)resource_from_handle_right[idx_map[pass_through4K_data->d3d11_texture2d_buffer_index]]);
            }
        }
        frame_idx = pass_through4K_data->output4k_frameSeq;
        time_idx = pass_through4K_data->output4k_timeStp;
    }
    img_left = undistorted_texture_left;
    img_right = undistorted_texture_right;

}

void ViveSR_DualCameraImageCapture::GetDepthTexture(UTexture2D*& imageDepth, int& frame_idx, int& time_idx)
{
	depth_texture->UpdateTextureRegions(0, 1, depth_texture_region, static_cast<uint32>(depth_img_width * sizeof(float) * depth_img_channel), sizeof(float), depth_raw, texCleanUpFP);
	imageDepth = depth_texture;
	frame_idx = depth_data->frame_seq;
	time_idx = depth_data->time_stp;
}

bool ViveSR_DualCameraImageCapture::UpdateDistortedImage()
{
    if (!img4K_ready) {
        get_PassThrough_result = ViveSR::SRWork::PassThrough::GetPassThroughData(pass_through_data);
        if (get_PassThrough_result != 0) {
            UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetPassThrough Data %d"), get_PassThrough_result);
            return false;
        }
        if (!is_gpu_path) {
            int length = 0;
            length = distorted_img_width * distorted_img_height * distorted_img_channel;
            memcpy(distorted_raw_bgra_left, pass_through_data->distorted_frame_left, length);
            memcpy(distorted_raw_bgra_right, pass_through_data->distorted_frame_right, length);
        }

        else {
            int buffer_idx = pass_through_data->d3d11_texture2d_buffer_index;
            if (!idx_map.Contains(buffer_idx)) {
                idx_map.Add(buffer_idx, vertex_idx);
                vertex_idx++;
                srv_left.resize(srv_left.size() + 1);
                srv_right.resize(srv_right.size() + 1);

                resource_from_handle_left.resize(resource_from_handle_left.size() + 1);
                resource_from_handle_right.resize(resource_from_handle_right.size() + 1);


                if (!GetD3D11ShaderResourceView((void*)pass_through_data->d3d11_texture2d_shared_handle_left, &srv_left[idx_map[buffer_idx]])){
                    idx_map.Remove(buffer_idx);
                    return false;
                }
                if (!GetD3D11ShaderResourceView((void*)pass_through_data->d3d11_texture2d_shared_handle_right, &srv_right[idx_map[buffer_idx]])) {
                    idx_map.Remove(buffer_idx);
                    return false;
                }
                srv_from_handle_left = (ID3D11ShaderResourceView*)srv_left[idx_map[buffer_idx]];
                srv_from_handle_left->GetResource(&resource_from_handle_left[idx_map[buffer_idx]]);
                srv_from_handle_right = (ID3D11ShaderResourceView*)srv_right[idx_map[buffer_idx]];
                srv_from_handle_right->GetResource(&resource_from_handle_right[idx_map[buffer_idx]]);
            }
        }

        distorted_pose_left = FMatrix(
            FPlane(pass_through_data->pose_left[0], pass_through_data->pose_left[4], pass_through_data->pose_left[8], pass_through_data->pose_left[12]),
            FPlane(pass_through_data->pose_left[1], pass_through_data->pose_left[5], pass_through_data->pose_left[9], pass_through_data->pose_left[13]),
            FPlane(pass_through_data->pose_left[2], pass_through_data->pose_left[6], pass_through_data->pose_left[10], pass_through_data->pose_left[14]),
            FPlane(pass_through_data->pose_left[3], pass_through_data->pose_left[7], pass_through_data->pose_left[11], pass_through_data->pose_left[15]));
        distorted_pose_right = FMatrix(
            FPlane(pass_through_data->pose_right[0], pass_through_data->pose_right[4], pass_through_data->pose_right[8], pass_through_data->pose_right[12]),
            FPlane(pass_through_data->pose_right[1], pass_through_data->pose_right[5], pass_through_data->pose_right[9], pass_through_data->pose_right[13]),
            FPlane(pass_through_data->pose_right[2], pass_through_data->pose_right[6], pass_through_data->pose_right[10], pass_through_data->pose_right[14]),
            FPlane(pass_through_data->pose_right[3], pass_through_data->pose_right[7], pass_through_data->pose_right[11], pass_through_data->pose_right[15]));
    }
    else {
        get_PassThrough_result = ViveSR::SRWork::PassThrough4K::GetPassThrough4KData(pass_through4K_data);    
        if (get_PassThrough_result != 0) {
            UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetPassThrough4K Data %d"), get_PassThrough_result);
            return false;

        }
        if (!is_gpu_path) {
            int length = 0;
            length = distorted_img_width * distorted_img_height * distorted_img_channel;
            memcpy(distorted_raw_bgra_left, pass_through4K_data->distorted_4k_frame_left, length);
            memcpy(distorted_raw_bgra_right, pass_through4K_data->distorted_4k_frame_right, length);
        }

        else {
            int buffer_idx = pass_through4K_data->d3d11_texture2d_buffer_index;
            if (!idx_map.Contains(buffer_idx)) {
                idx_map.Add(buffer_idx, vertex_idx);
                vertex_idx++;
                srv_left.resize(srv_left.size() + 1);
                srv_right.resize(srv_right.size() + 1);

                resource_from_handle_left.resize(resource_from_handle_left.size() + 1);
                resource_from_handle_right.resize(resource_from_handle_right.size() + 1);

                if (!GetD3D11ShaderResourceView((void*)pass_through4K_data->d3d11_texture2d_shared_handle_left, &srv_left[idx_map[buffer_idx]])) {
                    idx_map.Remove(buffer_idx);
                    return false;
                }
                if (!GetD3D11ShaderResourceView((void*)pass_through4K_data->d3d11_texture2d_shared_handle_right, &srv_right[idx_map[buffer_idx]])) {
                    idx_map.Remove(buffer_idx);
                    return false;
                }
                srv_from_handle_left = (ID3D11ShaderResourceView*)srv_left[idx_map[buffer_idx]];
                srv_from_handle_left->GetResource(&resource_from_handle_left[idx_map[buffer_idx]]);
                srv_from_handle_right = (ID3D11ShaderResourceView*)srv_right[idx_map[buffer_idx]];
                srv_from_handle_right->GetResource(&resource_from_handle_right[idx_map[buffer_idx]]);
            }
        }
        distorted_pose_left = FMatrix(
            FPlane(pass_through4K_data->output4k_pose_left[0], pass_through4K_data->output4k_pose_left[4], pass_through4K_data->output4k_pose_left[8], pass_through4K_data->output4k_pose_left[12]),
            FPlane(pass_through4K_data->output4k_pose_left[1], pass_through4K_data->output4k_pose_left[5], pass_through4K_data->output4k_pose_left[9], pass_through4K_data->output4k_pose_left[13]),
            FPlane(pass_through4K_data->output4k_pose_left[2], pass_through4K_data->output4k_pose_left[6], pass_through4K_data->output4k_pose_left[10], pass_through4K_data->output4k_pose_left[14]),
            FPlane(pass_through4K_data->output4k_pose_left[3], pass_through4K_data->output4k_pose_left[7], pass_through4K_data->output4k_pose_left[11], pass_through4K_data->output4k_pose_left[15]));
        distorted_pose_right = FMatrix(
            FPlane(pass_through4K_data->output4k_pose_right[0], pass_through4K_data->output4k_pose_right[4], pass_through4K_data->output4k_pose_right[8], pass_through4K_data->output4k_pose_right[12]),
            FPlane(pass_through4K_data->output4k_pose_right[1], pass_through4K_data->output4k_pose_right[5], pass_through4K_data->output4k_pose_right[9], pass_through4K_data->output4k_pose_right[13]),
            FPlane(pass_through4K_data->output4k_pose_right[2], pass_through4K_data->output4k_pose_right[6], pass_through4K_data->output4k_pose_right[10], pass_through4K_data->output4k_pose_right[14]),
            FPlane(pass_through4K_data->output4k_pose_right[3], pass_through4K_data->output4k_pose_right[7], pass_through4K_data->output4k_pose_right[11], pass_through4K_data->output4k_pose_right[15]));
    }
    return true;
}

bool ViveSR_DualCameraImageCapture::UpdateUndistortedImage()
{
    if (!img4K_ready) {
        get_PassThrough_result = ViveSR::SRWork::PassThrough::GetPassThroughData(pass_through_data);
        

        if (get_PassThrough_result != 0) {
            UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetPassThrough Data %d"), get_PassThrough_result);
            return false;
        }
        if (!is_gpu_path) {
            int length = 0;
            length = undistorted_img_width * undistorted_img_height * UndistortedImageChannel;
            memcpy(undistorted_raw_bgra_left, pass_through_data->undistorted_frame_left, length);
            memcpy(undistorted_raw_bgra_right, pass_through_data->undistorted_frame_right, length);
        }

        else {
            int buffer_idx = pass_through_data->d3d11_texture2d_buffer_index;        
            if (!idx_map.Contains(buffer_idx)) {
                idx_map.Add(buffer_idx, vertex_idx);
                vertex_idx++;
                srv_left.resize(srv_left.size() + 1);
                srv_right.resize(srv_right.size() + 1);

                resource_from_handle_left.resize(resource_from_handle_left.size() + 1);
                resource_from_handle_right.resize(resource_from_handle_right.size() + 1);


                if (!GetD3D11ShaderResourceView((void*)pass_through_data->d3d11_texture2d_shared_handle_left, &srv_left[idx_map[buffer_idx]])) {
                    idx_map.Remove(buffer_idx);
                    return false;
                }
                if (!GetD3D11ShaderResourceView((void*)pass_through_data->d3d11_texture2d_shared_handle_right, &srv_right[idx_map[buffer_idx]])) {
                    idx_map.Remove(buffer_idx);
                    return false;
                }
                srv_from_handle_left = (ID3D11ShaderResourceView*)srv_left[idx_map[buffer_idx]];
                srv_from_handle_left->GetResource(&resource_from_handle_left[idx_map[buffer_idx]]);
                srv_from_handle_right = (ID3D11ShaderResourceView*)srv_right[idx_map[buffer_idx]];
                srv_from_handle_right->GetResource(&resource_from_handle_right[idx_map[buffer_idx]]);
            }
        }

        undistorted_pose_left = FMatrix(
            FPlane(pass_through_data->pose_left[0], pass_through_data->pose_left[4], pass_through_data->pose_left[8], pass_through_data->pose_left[12]),
            FPlane(pass_through_data->pose_left[1], pass_through_data->pose_left[5], pass_through_data->pose_left[9], pass_through_data->pose_left[13]),
            FPlane(pass_through_data->pose_left[2], pass_through_data->pose_left[6], pass_through_data->pose_left[10], pass_through_data->pose_left[14]),
            FPlane(pass_through_data->pose_left[3], pass_through_data->pose_left[7], pass_through_data->pose_left[11], pass_through_data->pose_left[15]));
        undistorted_pose_right = FMatrix(
            FPlane(pass_through_data->pose_right[0], pass_through_data->pose_right[4], pass_through_data->pose_right[8], pass_through_data->pose_right[12]),
            FPlane(pass_through_data->pose_right[1], pass_through_data->pose_right[5], pass_through_data->pose_right[9], pass_through_data->pose_right[13]),
            FPlane(pass_through_data->pose_right[2], pass_through_data->pose_right[6], pass_through_data->pose_right[10], pass_through_data->pose_right[14]),
            FPlane(pass_through_data->pose_right[3], pass_through_data->pose_right[7], pass_through_data->pose_right[11], pass_through_data->pose_right[15]));
    }
    else {
        get_PassThrough_result = ViveSR::SRWork::PassThrough4K::GetPassThrough4KData(pass_through4K_data);
       

        if (get_PassThrough_result != 0) {
            UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetPassThrough4K Data %d"), get_PassThrough_result);
            return false;
        }
        if (!is_gpu_path) {
            int length = 0;
            length = undistorted_img_width * undistorted_img_height * UndistortedImageChannel;
            memcpy(undistorted_raw_bgra_left, pass_through4K_data->undistorted_4k_frame_left, length);
            memcpy(undistorted_raw_bgra_right, pass_through4K_data->undistorted_4k_frame_right, length);
        }

        else {
            
            int buffer_idx = pass_through4K_data->d3d11_texture2d_buffer_index;
            if (!idx_map.Contains(buffer_idx)) {
                idx_map.Add(buffer_idx, vertex_idx);
                vertex_idx++;
                srv_left.resize(srv_left.size() + 1);
                srv_right.resize(srv_right.size() + 1);

                resource_from_handle_left.resize(resource_from_handle_left.size() + 1);
                resource_from_handle_right.resize(resource_from_handle_right.size() + 1);

                if (!GetD3D11ShaderResourceView((void*)pass_through4K_data->d3d11_texture2d_shared_handle_left, &srv_left[idx_map[buffer_idx]])) {
                    idx_map.Remove(buffer_idx);
                    return false;
                }
                if (!GetD3D11ShaderResourceView((void*)pass_through4K_data->d3d11_texture2d_shared_handle_right, &srv_right[idx_map[buffer_idx]])) {
                    idx_map.Remove(buffer_idx);
                    return false;
                }
                srv_from_handle_left = (ID3D11ShaderResourceView*)srv_left[idx_map[buffer_idx]];
                srv_from_handle_left->GetResource(&resource_from_handle_left[idx_map[buffer_idx]]);
                srv_from_handle_right = (ID3D11ShaderResourceView*)srv_right[idx_map[buffer_idx]];
                srv_from_handle_right->GetResource(&resource_from_handle_right[idx_map[buffer_idx]]);
            }      
        }
        undistorted_pose_left = FMatrix(
            FPlane(pass_through4K_data->output4k_pose_left[0], pass_through4K_data->output4k_pose_left[4], pass_through4K_data->output4k_pose_left[8], pass_through4K_data->output4k_pose_left[12]),
            FPlane(pass_through4K_data->output4k_pose_left[1], pass_through4K_data->output4k_pose_left[5], pass_through4K_data->output4k_pose_left[9], pass_through4K_data->output4k_pose_left[13]),
            FPlane(pass_through4K_data->output4k_pose_left[2], pass_through4K_data->output4k_pose_left[6], pass_through4K_data->output4k_pose_left[10], pass_through4K_data->output4k_pose_left[14]),
            FPlane(pass_through4K_data->output4k_pose_left[3], pass_through4K_data->output4k_pose_left[7], pass_through4K_data->output4k_pose_left[11], pass_through4K_data->output4k_pose_left[15]));
        undistorted_pose_right = FMatrix(
            FPlane(pass_through4K_data->output4k_pose_right[0], pass_through4K_data->output4k_pose_right[4], pass_through4K_data->output4k_pose_right[8], pass_through4K_data->output4k_pose_right[12]),
            FPlane(pass_through4K_data->output4k_pose_right[1], pass_through4K_data->output4k_pose_right[5], pass_through4K_data->output4k_pose_right[9], pass_through4K_data->output4k_pose_right[13]),
            FPlane(pass_through4K_data->output4k_pose_right[2], pass_through4K_data->output4k_pose_right[6], pass_through4K_data->output4k_pose_right[10], pass_through4K_data->output4k_pose_right[14]),
            FPlane(pass_through4K_data->output4k_pose_right[3], pass_through4K_data->output4k_pose_right[7], pass_through4K_data->output4k_pose_right[11], pass_through4K_data->output4k_pose_right[15]));
    }	
    return true;
}

bool ViveSR_DualCameraImageCapture::UpdateDepthImage()
{
	int get_depth_result = ViveSR::SRWork::Depth::GetDepthData(depth_data);

	if (get_depth_result == ViveSR::Error::WORK)
	{
		depth_pose = FMatrix(
			FPlane(depth_data->pose[0], depth_data->pose[4], depth_data->pose[8], depth_data->pose[12]),
			FPlane(depth_data->pose[1], depth_data->pose[5], depth_data->pose[9], depth_data->pose[13]),
			FPlane(depth_data->pose[2], depth_data->pose[6], depth_data->pose[10], depth_data->pose[14]),
			FPlane(depth_data->pose[3], depth_data->pose[7], depth_data->pose[11], depth_data->pose[15]));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] Update DEPTH Error %d"), get_depth_result);
        return false;
	}
	int length = depth_img_width * depth_img_height * depth_img_channel * depth_data_size;
	memcpy(depth_raw, depth_data->depth_map, length);
    return true;
}
bool ViveSR_DualCameraImageCapture::GetD3D11ShaderResourceView(void* texture_shared_handle, void** shader_resource_view) {
    if (!native_device)
    {
        UE_LOG(LogTemp, Warning, TEXT("native_device is nullptr"));
        return false;
    }

    HANDLE* shared_handle = (HANDLE*)texture_shared_handle;
    IDXGIResource* resource = nullptr;
    ID3D11Texture2D* texture = nullptr;
    ID3D11ShaderResourceView* _shader_resource_view = nullptr;
   
    HRESULT hr = native_device->OpenSharedResource(*shared_handle, __uuidof(ID3D11Texture2D), (void **)&resource);
   
    if (FAILED(hr)) {
        UE_LOG(LogTemp, Warning, TEXT("OpenSharedResource() failed"));
        return false;
    }
    hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), (void **)&texture);
    resource->Release();
    if (FAILED(hr)) {
        UE_LOG(LogTemp, Warning, TEXT("Get texture failed"));
        return false;
    }
    hr = native_device->CreateShaderResourceView(texture, NULL, &_shader_resource_view);
    if (FAILED(hr)) {
        UE_LOG(LogTemp, Warning, TEXT("CreateShaderResourceView() failed"));
        return false;
    }
    *shader_resource_view = (void *)_shader_resource_view;
    return true;
}

void ViveSR_DualCameraImageCapture::SetGPUMethod(bool GPUSetting) {
    is_gpu_path = GPUSetting;
}
#pragma region purple fringe removal
bool ViveSR_DualCameraImageCapture::IsHorus() {
    int system_device = 0;
    ViveSR::SRWork::PassThrough::GetPassThrougParameterInt(ViveSR::PassThrough::Param::DEVICE_SYSTEM_TYPE, &system_device);
    if (ViveSR::VRDevice::COSMOS == system_device && !img4K_ready) {
        return true;
    }
    return false;
}
float ViveSR_DualCameraImageCapture::GetGamma()
{
    return (pass_through_data->gamma);
}
#pragma endregion
#pragma region Utility
FVector ViveSR_DualCameraImageCapture::Position(FMatrix m)
{
	return FVector(m.M[2][3], m.M[0][3], m.M[1][3]);
}
FRotator ThreeAxisRot(double r11, double r12, double r21, double r31, double r32)
{
	return FRotator(atan2(r31, r32), asin(r21), atan2(r11, r12));
}
FRotator Quaternion2Euler(FQuat q)
{
	return ThreeAxisRot(-2 * (q.X*q.Y - q.W*q.Z),
		q.W*q.W - q.X*q.X + q.Y*q.Y - q.Z*q.Z,
		2 * (q.Y*q.Z + q.W*q.X),
		-2 * (q.X*q.Z - q.W*q.Y),
		q.W*q.W - q.X*q.X - q.Y*q.Y + q.Z*q.Z);
}
FRotator FMatrixToRotator(FMatrix pose)
{
	if (!((FMath::Abs(1.f - pose.GetScaledAxis(EAxis::X).SizeSquared()) <= KINDA_SMALL_NUMBER) && (FMath::Abs(1.f - pose.GetScaledAxis(EAxis::Y).SizeSquared()) <= KINDA_SMALL_NUMBER) && (FMath::Abs(1.f - pose.GetScaledAxis(EAxis::Z).SizeSquared()) <= KINDA_SMALL_NUMBER)))
	{
		pose.RemoveScaling(KINDA_SMALL_NUMBER);
	}
	FQuat Orientation(pose);
	FRotator ZXY = Quaternion2Euler(Orientation);
	return FRotator(FMath::RadiansToDegrees(ZXY.Yaw),
		-FMath::RadiansToDegrees(ZXY.Pitch),
		FMath::RadiansToDegrees(ZXY.Roll));
}
#pragma endregion

FVector ViveSR_DualCameraImageCapture::GetDistortedLocalPosition(DualCameraIndex eye)
{
	if (eye == DualCameraIndex::LEFT)
		return Position(distorted_pose_left);
	else if (eye == DualCameraIndex::RIGHT)
		return Position(distorted_pose_right);
	else
		return FVector(0, 0, 0);
}
FRotator ViveSR_DualCameraImageCapture::GetDistortedLocalRotation(DualCameraIndex eye)
{
	if (eye == DualCameraIndex::LEFT)
		return FMatrixToRotator(distorted_pose_left);
	else if (eye == DualCameraIndex::RIGHT)
		return FMatrixToRotator(distorted_pose_right);
	else
		return FRotator(0, 0, 0);
}
FVector ViveSR_DualCameraImageCapture::GetUndistortedLocalPosition(DualCameraIndex eye)
{
	if (eye == DualCameraIndex::LEFT)
		return Position(undistorted_pose_left);
	else if (eye == DualCameraIndex::RIGHT)
		return Position(undistorted_pose_right);
	else
		return FVector(0, 0, 0); 
}
FRotator ViveSR_DualCameraImageCapture::GetUndistortedLocalRotation(DualCameraIndex eye)
{
	if (eye == DualCameraIndex::LEFT)
		return FMatrixToRotator(undistorted_pose_left);
	else if (eye == DualCameraIndex::RIGHT)
		return FMatrixToRotator(undistorted_pose_right);
	else
		return FRotator(0, 0, 0);   
}

FVector ViveSR_DualCameraImageCapture::GetDepthLocalPosition()
{
	return Position(depth_pose);
}
FRotator ViveSR_DualCameraImageCapture::GetDepthLocalRotation()
{
	return FMatrixToRotator(depth_pose);
}


#pragma region Depth LinkMethod / Commands / Parameters
int ViveSR_DualCameraImageCapture::EnableDepthProcess(bool active)
{
    int result = ViveSR::Error::FAILED;
    if (active)
    {
        ViveSR::SRWork::PassThrough::SetSkipVGAProcess(false);
        result = ViveSR::SRWork::LinkModule(ViveSR::SRWork::PassThrough::MODULE_PASS_THROUGHT_TYPE, ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE);
        if (result == ViveSR::Error::WORK)
        {
            result = ViveSR::SRWork::PassThrough::TurnOnUndistortDataToDepth();
            is_depth_processing = true;
        }
        else is_depth_processing = false;
    }
    else
    {
        result = ViveSR::SRWork::UnlinkModule(ViveSR::SRWork::PassThrough::MODULE_PASS_THROUGHT_TYPE, ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE);
        if (result == ViveSR::Error::WORK)
        {
            result = ViveSR::SRWork::PassThrough::TurnOffUndistortDataToDepth();
            is_depth_processing = false;
        }
    }
    return result;
}
int ViveSR_DualCameraImageCapture::EnableDepthRefinement(bool active)
{
	int result = ViveSR::SRWork::Depth::SetDepthParameterBool(ViveSR::Depth::Cmd::ENABLE_REFINEMENT, active);
	if (result == ViveSR::Error::WORK) is_depth_refinement_enabled = active;
	return result;
}
bool ViveSR_DualCameraImageCapture::IsDepthRefinementActive()
{
    bool active;
    int result = ViveSR::SRWork::Depth::GetDepthParameterBool(ViveSR::Depth::Cmd::ENABLE_REFINEMENT, &active);
    return active;
}
int ViveSR_DualCameraImageCapture::EnableDepthEdgeEnhance(bool active)
{
	int result = ViveSR::SRWork::Depth::SetDepthParameterBool(ViveSR::Depth::Cmd::ENABLE_EDGE_ENHANCE, active);
	if (result == ViveSR::Error::WORK) is_depth_edge_enhance_enabled = active;
	return result;
}
int ViveSR_DualCameraImageCapture::SetDepthCase(ViveSR::Depth::DepthCase depthCase)
{
	int result = ViveSR::Error::FAILED;
	result = ViveSR::SRWork::Depth::SetDepthParameterInt( ViveSR::Depth::Cmd::CHANGE_DEPTH_CASE, (int)depthCase);
	if (result == ViveSR::Error::WORK) depth_case = depthCase;
	return result;
}

int ViveSR_DualCameraImageCapture::SetDepthConfidenceThreshold(float value)
{
	int result = ViveSR::SRWork::Depth::SetDepthParameterFloat( ViveSR::Depth::Param::CONFIDENCE_THRESHOLD, value);
	
	if (result == ViveSR::Error::WORK) depth_confidence_threshold = value;
	return result;
}
int ViveSR_DualCameraImageCapture::SetDepthDenoiseGuidedFilter(int value)
{
	int result = ViveSR::SRWork::Depth::SetDepthParameterInt( ViveSR::Depth::Param::DENOISE_GUIDED_FILTER, value);
	if (result == ViveSR::Error::WORK) depth_denoise_guided_filter = value;
	return result;
}
int ViveSR_DualCameraImageCapture::SetDepthDenoiseMedianFilter(int value)
{
	int result = ViveSR::SRWork::Depth::SetDepthParameterInt(ViveSR::Depth::Param::DENOISE_MEDIAN_FILTER, value);
	if (result == ViveSR::Error::WORK) depth_denoise_median_filter = value;
	return result;
}
#pragma endregion

#pragma region Camera Commands / Parameters
int ViveSR_DualCameraImageCapture::GetCameraSettings(ViveSR::PassThrough::CameraControlType item, ViveSR::PassThrough::CameraSettings *param_info)
{
	int result = (int)ViveSR::Error::FAILED;
	result = ViveSR::SRWork::PassThrough::GetCameraStatus(item, &(param_info->Status));
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetCameraStatus %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::GetCameraDefaultValue(item, &(param_info->DefaultValue));
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetCameraDefaultValue %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::GetCameraMin(item, &(param_info->Min));
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetCameraMin %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::GetCameraMax(item, &(param_info->Max));
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetCameraMax %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::GetCameraStep(item, &(param_info->Step));
	if (result != (int)ViveSR::Error::WORK || &(param_info->Step) == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetCameraStep %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::GetCameraDefaultMode(item, &(param_info->DefaultMode));
	if (result != (int)ViveSR::Error::WORK || &(param_info->DefaultMode) == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetCameraDefaultMode %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::GetCameraValue(item, &(param_info->Value));
	if (result != (int)ViveSR::Error::WORK || &(param_info->Value) == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetCameraValue %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::GetCameraMode(item, &(param_info->Mode));
	if (result != (int)ViveSR::Error::WORK || &(param_info->Mode) == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetCameraMode %d"), result);
		return result;
	}
	UE_LOG(LogTemp, Warning, TEXT("[ViveSR] GetCameraQuality %d"), result);
	return result;
}
int ViveSR_DualCameraImageCapture::SetCamera(ViveSR::PassThrough::CameraControlType item, ViveSR::PassThrough::CameraSettings *param_info)
{
	int result = (int)ViveSR::Error::FAILED;
	result = ViveSR::SRWork::PassThrough::SetCameraStatus(item, (param_info->Status));
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] SetCameraStatus %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::SetCameraDefaultValue(item, (param_info->DefaultValue));
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] SetCameraDefaultValue %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::SetCameraMin(item, (param_info->Min));
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] SetCameraMin %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::SetCameraMax(item, param_info->Max);
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] SetCameraMax %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::SetCameraStep(item, param_info->Step);
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] SetCameraStep %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::SetCameraDefaultMode(item, param_info->DefaultMode);
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] SetCameraDefaultMode %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::SetCameraValue(item, param_info->Value);
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] SetCameraValue %d"), result);
		return result;
	}
	result = ViveSR::SRWork::PassThrough::SetCameraMode(item, param_info->Mode);
	if (result != (int)ViveSR::Error::WORK) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] SetCameraMode %d"), result);
		return result;
	}
	UE_LOG(LogTemp, Warning, TEXT("[ViveSR] SetCameraQuality %d"), result);
	return result;
}
#pragma endregion
