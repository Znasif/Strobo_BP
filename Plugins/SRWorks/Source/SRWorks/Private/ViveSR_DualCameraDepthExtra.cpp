// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_DualCameraDepthExtra.h"
#include "ViveSR_DualCameraImageCapture.h"
#include "ViveSR_Enums.h"
#include "SRWorks.h"
#include <algorithm>

ViveSR_DualCameraDepthExtra* ViveSR_DualCameraDepthExtra::Mgr = nullptr;
ViveSR_DualCameraDepthExtra::ViveSR_DualCameraDepthExtra()
{
}
ViveSR_DualCameraDepthExtra::~ViveSR_DualCameraDepthExtra()
{
}
ViveSR_DualCameraDepthExtra* ViveSR_DualCameraDepthExtra::Instance()
{
	if (Mgr == nullptr)
	{
		Mgr = new ViveSR_DualCameraDepthExtra();
	}
	return Mgr;
}
void ViveSR_DualCameraDepthExtra::Destroy()
{
	if (Mgr != nullptr)
	{
		delete Mgr;
		Mgr = nullptr;
	}
}

void ViveSR_DualCameraDepthExtra::InitialDepthCollider(int depthImageWidth, int depthImageHeight)
{
	vertices_raw.Init(0, depthImageWidth * depthImageHeight * 3);
	indices_raw.Init(0, depthImageWidth * depthImageHeight * 6);

	depth_mesh_data = new ViveSR::SRWork::DepthMesh::DepthMeshData();
	depth_mesh_data->pose = new float[16];
	depth_mesh_data->vertices = new float[640 * 480 * 3];
	depth_mesh_data->indices = new int[640 * 480 * 6];
	depth_mesh_data->camera_params = new char[1032];
}
void ViveSR_DualCameraDepthExtra::Release() {
	vertices_raw.Empty();
	indices_raw.Empty();	
	if (depth_mesh_data) {
        if (depth_mesh_data->pose) {
            delete[] depth_mesh_data->pose; depth_mesh_data->pose = nullptr;
        }
        if (depth_mesh_data->vertices) {
            delete[] depth_mesh_data->vertices; depth_mesh_data->vertices = nullptr;
        }
        if (depth_mesh_data->indices) {
            delete[] depth_mesh_data->indices; depth_mesh_data->indices = nullptr;
        }
        if (depth_mesh_data->camera_params) {
            delete[] depth_mesh_data->camera_params; depth_mesh_data->camera_params = nullptr;
        }
	}
    if (nullptr != depth_mesh_data) {
        delete depth_mesh_data;
        depth_mesh_data = nullptr;
    }
}
bool ViveSR_DualCameraDepthExtra::UpdateDepthMeshData() {
	int get_depth_mesh_result = ViveSR::SRWork::DepthMesh::GetDepthMeshData(depth_mesh_data);
	if (get_depth_mesh_result != ViveSR::Error::WORK)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] Update DEPTHCOLLIDER Error %d"), get_depth_mesh_result);
	}
	return get_depth_mesh_result == ViveSR::Error::WORK;
}
bool ViveSR_DualCameraDepthExtra::GetDepthColliderFrameInfo()
{
	time_index = depth_mesh_data->time_stp;
	if (last_time_idx == time_index) return false;
	else last_time_idx = time_index;
	return true;
}

bool ViveSR_DualCameraDepthExtra::GetDepthColliderData(int &verticesNum, TArray<float> &verticesBuff, int &indicesNum, TArray<int> &indicesBuff)
{	
	verticesNum = depth_mesh_data->num_vertices;
	indicesNum = depth_mesh_data->num_indices;
	
	byte_vertex_raw = depth_mesh_data->bytepervert;
	std::copy(depth_mesh_data->vertices, depth_mesh_data->vertices + depth_mesh_data->num_vertices * byte_vertex_raw / 3, vertices_raw.GetData());
	std::copy(depth_mesh_data->indices, depth_mesh_data->indices + depth_mesh_data->num_indices, indices_raw.GetData());
	verticesBuff = vertices_raw;
	indicesBuff = indices_raw;
	return ViveSR::Error::WORK;
}