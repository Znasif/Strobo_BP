// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViveSR_RigidReconstruction.h"
#include "ViveSR_UEnums.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeCounter.h"

/**
*
*/
class ViveSR_URigidReconstruction : public FRunnable
{
public:
	ViveSR_URigidReconstruction();
	~ViveSR_URigidReconstruction();
	static ViveSR_URigidReconstruction* Instance();

	void Initial();
	void Release();

	virtual bool Init(); // FRunnable interface
	virtual uint32 Run();// FRunnable interface
	virtual void Stop(); // FRunnable interface

	int GetRigidReconstructionIntParameter(int type);
	void AllocOutputDataMemory();
	void FreeOutputDataMemory();
	bool GetRigidReconstructionFrame(int &frame);
	bool GetRigidReconstructionData(int &frame, TArray<float> &pose, int &verticesNum, TArray<float> &verticesBuff, int &vertStrideInFloat, int &indicesNum, TArray<int> &indicesBuff);
	void StartExporting(FString filename);
	void GetExportProgress(int &stage, int &percentage);
	void UpdateExportProgress();
	int  GetExportProgress();
	void EnableLiveMeshExtraction(bool enable);
	void StartScanning();
	void StopScanning();
    int EnableReconstructionProcess(bool active);
    int ResetReconstructionEngine();

	bool export_mesh;
	float adaptive_grid_size_max;
	float adaptive_grid_size_min;
	float adaptive_error_thres;
	bool is_scanning = false;
	bool has_scanned = true;
	bool is_exporting = false;
	int exporting_stage;
	int exporting_percent;
    int get_data_result;
    bool is_reconstruction_processing = false;
private:
	static ViveSR_URigidReconstruction *Runnable;
	bool is_initial = false;

	TArray<float> out_vertex;
	TArray<int> out_index;
	TArray<float> raw_tracked_pose;
	
	void UpdateFrame();
	void UpdateData();

    int StartReconstructionModule();
    int StopReconstructionModule();

	//Thread to run the worker FRunnable on
	FRunnableThread* Thread;
	/** Stop this thread? Uses Thread Safe Counter */
	FThreadSafeCounter StopTaskCounter;
	FThreadSafeCounter WorkerCounter;
	FCriticalSection m_mutex;
	ViveSR::SRWork::RigidReconstruction::RigidReconstructionData* reconstruction_data = nullptr;
	char* parameter;
};
