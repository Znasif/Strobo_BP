// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_URigidReconstruction.h"
#include "SRWorks.h"
#include "SRWork_Client_API.h"
#include "ViveSR_PassThrough.h"
#include "ViveSR_Depth.h"
#include "ViveSR_DepthEnums.h"
#include "ViveSR_Enums.h"
#include "Engine.h"
#include "Modules/ModuleManager.h"
#include "Containers/StringConv.h"
#include <algorithm> //for std copy

#pragma region singleton 
ViveSR_URigidReconstruction* ViveSR_URigidReconstruction::Runnable = nullptr;
ViveSR_URigidReconstruction::ViveSR_URigidReconstruction()
{

}
ViveSR_URigidReconstruction::~ViveSR_URigidReconstruction()
{
}
ViveSR_URigidReconstruction* ViveSR_URigidReconstruction::Instance()
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new ViveSR_URigidReconstruction();
	}
	return Runnable;
}
void ViveSR_URigidReconstruction::Initial()
{
	FString ThreadName(FString::Printf(TEXT("ViveSR_URigidReconstruction%i"), Runnable->WorkerCounter.Increment()));
	Runnable->Thread = FRunnableThread::Create(Runnable, *ThreadName, 0, TPri_BelowNormal);
	Runnable->Thread->Suspend(true);
	is_initial = true;
	is_scanning = false;
	has_scanned = true;
	is_exporting = false;
}
void ViveSR_URigidReconstruction::Release()
{
	if (!is_initial) return;
	if (Runnable)
	{
		Runnable->Stop();
		FreeOutputDataMemory();
		Runnable->Thread->Suspend(false);
		Runnable->Thread->WaitForCompletion();
		Runnable->Thread->Kill(true);
		delete Runnable;
		Runnable = nullptr;
	}
	is_initial = false;
}
#pragma endregion


void ViveSR_URigidReconstruction::UpdateFrame()
{
    get_data_result = ViveSR::SRWork::RigidReconstruction::GetRigidReconstructionData(reconstruction_data);
	if (get_data_result != ViveSR::Error::WORK)
	{
		UE_LOG(LogTemp, Warning,TEXT("[ViveSR] Update RigidReconstruction Error %d"), get_data_result);
	}
}
void ViveSR_URigidReconstruction::UpdateData()
{
	int result = ViveSR::SRWork::RigidReconstruction::GetRigidReconstructionData(reconstruction_data);
	if (result != ViveSR::Error::WORK)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] Update RigidReconstruction Error %d"), result);
	}
}

#pragma region Public APIs
int ViveSR_URigidReconstruction::GetRigidReconstructionIntParameter(int type)
{
	int ret = -1;
	if (ViveSR::SRWork::RigidReconstruction::GetReconstructionParameterInt(type, &ret) != (int)ViveSR::Error::WORK)
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] [RigidReconstruction] GetRigidReconstructionIntParameter Failed"));
	return ret;
}

void ViveSR_URigidReconstruction::AllocOutputDataMemory()
{
	out_vertex.Init(0, 8 * 2500000);
	out_index.Init(0, 2500000);
	raw_tracked_pose.Init(0, 16);

	export_mesh = true;
	adaptive_grid_size_max = 64;
	adaptive_grid_size_min = 4;
	adaptive_error_thres = 0.4f;
	reconstruction_data = new ViveSR::SRWork::RigidReconstruction::RigidReconstructionData();
	reconstruction_data->posemtx44 = new float[16];
	reconstruction_data->vertices = new float[8 * 2500000];
	reconstruction_data->indices = new int[2500000];
	reconstruction_data->cld_num_verts = new unsigned int[200];
	reconstruction_data->cld_numidx = new unsigned int[200];
	reconstruction_data->cld_vertices = new float[3 * 50000];
	reconstruction_data->cld_indices = new int[100000];
	reconstruction_data->sector_id_list = new int[1000000];
	reconstruction_data->sector_vert_num = new int[1000000];
	reconstruction_data->sector_idx_num = new int[1000000];

}

void  ViveSR_URigidReconstruction::FreeOutputDataMemory() {
	if (reconstruction_data) {
        if (reconstruction_data->posemtx44) {
            delete[] reconstruction_data->posemtx44; reconstruction_data->posemtx44 = nullptr;
        }
        if (reconstruction_data->vertices) {
            delete[] reconstruction_data->vertices; reconstruction_data->vertices = nullptr;
        }
        if (reconstruction_data->indices) {
            delete[] reconstruction_data->indices; reconstruction_data->indices = nullptr;
        }
        if (reconstruction_data->cld_num_verts) {
            delete[] reconstruction_data->cld_num_verts; reconstruction_data->cld_num_verts = nullptr;
        }
        if (reconstruction_data->cld_numidx) {
            delete[] reconstruction_data->cld_numidx; reconstruction_data->cld_numidx = nullptr;
        }
        if (reconstruction_data->cld_vertices) {
            delete[] reconstruction_data->cld_vertices; reconstruction_data->cld_vertices = nullptr;
        }
        if (reconstruction_data->cld_indices) {
            delete[] reconstruction_data->cld_indices; reconstruction_data->cld_indices = nullptr;
        }
        if (reconstruction_data->sector_id_list) {
            delete[] reconstruction_data->sector_id_list; reconstruction_data->sector_id_list = nullptr;
        }
        if (reconstruction_data->sector_vert_num) {
            delete[] reconstruction_data->sector_vert_num; reconstruction_data->sector_vert_num = nullptr;
        }
        if (reconstruction_data->sector_idx_num) {
            delete[] reconstruction_data->sector_idx_num; reconstruction_data->sector_idx_num = nullptr;
        }
	}
    if (reconstruction_data) { delete[] reconstruction_data; reconstruction_data = nullptr; }
	
}

void ViveSR_URigidReconstruction::UpdateExportProgress() {
	if (ViveSR::SRWork::RigidReconstruction::GetExportMeshProgress(&exporting_percent) == 0) {
		UE_LOG(LogTemp, Warning, TEXT("[ViveSR] [RigidReconstruction] exporting_percent %d"), exporting_percent);
		if (exporting_percent == 100)
			is_exporting = false;
	}
}
void ViveSR_URigidReconstruction::StartExporting(FString filename)
{
	exporting_stage = 0;
	exporting_percent = 0;
	is_scanning = false;	

	ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Param::EXPORT_ADAPTIVE_MODEL, export_mesh);
	if (export_mesh)
	{
		ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterFloat(ViveSR::RigidReconstruction::Param::ADAPTIVE_MAX_GRID, adaptive_grid_size_max * 0.01f);   // cm to m
		ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterFloat(ViveSR::RigidReconstruction::Param::ADAPTIVE_MIN_GRID, adaptive_grid_size_min * 0.01f);
		ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterFloat(ViveSR::RigidReconstruction::Param::ADAPTIVE_ERROR_THRES, adaptive_error_thres);
	}

	FString app_path = FPlatformProcess::UserSettingsDir();
	app_path.RemoveFromEnd("Local/");
	FString fullpath = app_path + "LocalLow/HTC Corporation/SR_Reconstruction_Output/";

	ViveSR::SRWork::RigidReconstruction::SetReconstructionOutputFolder((char*)StringCast<ANSICHAR>(*fullpath).Get(), fullpath.Len() + 1);
	ViveSR::SRWork::RigidReconstruction::SetReconstructionOutputFileName((char*)StringCast<ANSICHAR>(*filename).Get(), filename.Len() + 1);
	is_exporting = true;
	
}
void ViveSR_URigidReconstruction::GetExportProgress(int &stage, int &percentage)
{
	stage = exporting_stage;
	percentage = exporting_percent;
}
int ViveSR_URigidReconstruction::GetExportProgress()
{
	return exporting_percent;
}
void ViveSR_URigidReconstruction::EnableLiveMeshExtraction(bool enable)
{
	ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Cmd::EXTRACT_POINT_CLOUD, enable);
}

void ViveSR_URigidReconstruction::StartScanning()
{
	if (is_scanning) return;
    EnableReconstructionProcess(true);
    if (is_reconstruction_processing) {
        ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Cmd::START, true);
        is_scanning = true;
        Runnable->Thread->Suspend(false);
    }
}
void ViveSR_URigidReconstruction::StopScanning()
{
	if (!is_scanning) return;
    if (is_reconstruction_processing)
    {
        is_scanning = false;
        is_exporting = false;

        ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Cmd::STOP, true);
        Runnable->Thread->Suspend(true);
    }
    EnableReconstructionProcess(false);
}
int ViveSR_URigidReconstruction::ResetReconstructionEngine() {
    return ViveSR::SRWork::RigidReconstruction::ResetReconstructionModule();
}
bool ViveSR_URigidReconstruction::GetRigidReconstructionFrame(int &frame)
{
	if (Runnable) {
		Runnable->m_mutex.Lock();
		frame = Runnable->reconstruction_data->frame_seq;
		Runnable->m_mutex.Unlock();
	}
	return true;
}
bool ViveSR_URigidReconstruction::GetRigidReconstructionData(int &frame, TArray<float> &pose, int &verticesNum, TArray<float> &verticesBuff, int &vertStrideInFloat, int &indicesNum, TArray<int> &indicesBuff)
{
	if (Runnable) {
		if (Runnable->reconstruction_data->num_vertices > 0)
		{
			Runnable->m_mutex.Lock();
			frame = Runnable->reconstruction_data->frame_seq;
			verticesNum = Runnable->reconstruction_data->num_vertices;
			vertStrideInFloat = Runnable->reconstruction_data->bytepervert;
			indicesNum = Runnable->reconstruction_data->num_indices;
			std::copy(Runnable->reconstruction_data->posemtx44, Runnable->reconstruction_data->posemtx44 + 16, Runnable->raw_tracked_pose.GetData());
			std::copy(Runnable->reconstruction_data->vertices, Runnable->reconstruction_data->vertices + 8 * 2500000, Runnable->out_vertex.GetData());
			verticesBuff = Runnable->out_vertex;
			std::copy(Runnable->reconstruction_data->indices, Runnable->reconstruction_data->indices + 2500000, Runnable->out_index.GetData());
			indicesBuff = Runnable->out_index;
			pose = Runnable->raw_tracked_pose;
			Runnable->m_mutex.Unlock();
			return true;
		}
	}
	return false;
}
int ViveSR_URigidReconstruction::EnableReconstructionProcess(bool active) {
    int result = ViveSR::Error::FAILED;
    if (active)
    {
        result = StartReconstructionModule();
        if (result == ViveSR::Error::WORK) is_reconstruction_processing = true;
        else is_reconstruction_processing = false;
    }
    else
    {
        result = StopReconstructionModule();
        is_reconstruction_processing = false;
    }
    return result;
}
#pragma endregion
#pragma region Link Unlink Module
int ViveSR_URigidReconstruction::StartReconstructionModule() {
    ViveSR::SRWork::PassThrough::SetSkipVGAProcess(false);
    ViveSR::SRWork::Depth::SetDepthParameterBool(ViveSR::Depth::Cmd::CHANGE_DEPTH_CASE_2_RECONSTRUCT_USED, true);
    return ViveSR::SRWork::LinkModule(ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE, ViveSR::SRWork::RigidReconstruction::MODULE_RIGID_RECONSTRUCTION_TYPE);
}
int ViveSR_URigidReconstruction::StopReconstructionModule() {
    ViveSR::SRWork::Depth::SetDepthParameterBool(ViveSR::Depth::Cmd::CHANGE_DEPTH_CASE_2_RECONSTRUCT_USED, false);
    return ViveSR::SRWork::UnlinkModule(ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE, ViveSR::SRWork::RigidReconstruction::MODULE_RIGID_RECONSTRUCTION_TYPE);
}
#pragma endregion
#pragma region Thread Lifecycle
bool ViveSR_URigidReconstruction::Init()
{
	StopTaskCounter.Reset();
	return true;
}
uint32 ViveSR_URigidReconstruction::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	while (StopTaskCounter.GetValue() == 0)
	{
		m_mutex.Lock();
		if (is_scanning)
		{
			UpdateFrame();
		}
		m_mutex.Unlock();
		FPlatformProcess::Sleep(0.1);
	}
	return 0;
}
void ViveSR_URigidReconstruction::Stop()
{
	StopTaskCounter.Increment();
}
#pragma endregion
