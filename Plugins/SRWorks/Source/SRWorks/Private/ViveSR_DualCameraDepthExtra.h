// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SRWork_Client_API.h"
#include "Containers/Array.h"
#include "ViveSR_DepthMesh.h"
/**
 * 
 */
class ViveSR_DualCameraDepthExtra
{
public:
	ViveSR_DualCameraDepthExtra();
	~ViveSR_DualCameraDepthExtra();
	static ViveSR_DualCameraDepthExtra* Instance();
	static void Destroy();
	void InitialDepthCollider(int depthImageWidth, int depthImageHeight);
	void Release();
	bool GetDepthColliderFrameInfo();
	bool GetDepthColliderData(int &verticesNum, TArray<float> &verticesBuff, int &indicesNum, TArray<int> &indicesBuff);
	bool UpdateDepthMeshData();

    unsigned int time_index;
private:
	static ViveSR_DualCameraDepthExtra *Mgr;
	ViveSR::SRWork::DepthMesh::DepthMeshData *depth_mesh_data = nullptr;
	TArray<float> vertices_raw;
	TArray<int> indices_raw;

	int byte_vertex_raw;
	int last_time_idx = -1;
};
