// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ViveSR_Enums.h"
#include "ViveSR_ControllerPose.h"
#include <ctime>
#include <chrono>



struct PoseWithTimestamp
{
    float time_stamp;
    FVector position;
    FQuat rotation;
};

struct ObjectToMoveData
{
    FVector position;
    FQuat rotation;
    FVector local_position;
    FVector pos;
    FVector rot;
};

class ViveSR_ControllerLatencyTool
{
public:
    ViveSR_ControllerLatencyTool();
    ~ViveSR_ControllerLatencyTool();

    static ViveSR_ControllerLatencyTool* Instance();
    void Initial();
    void Release();
    int GetControllerTrackingPose();
    void AddPoseToMap(FVector position, FQuat rot);
    void SetStartTime(std::chrono::time_point<std::chrono::steady_clock> start);
    PoseWithTimestamp GetLatencyPosition(float timeDelay);

    ViveSR::SRWork::Controller::ControllerPoseData *pose_info = nullptr;
    TArray<PoseWithTimestamp> latency_map;
 
private:
    static ViveSR_ControllerLatencyTool *Mgr;
    
    std::chrono::time_point<std::chrono::steady_clock> start, current;
   
    std::chrono::duration<double> elapsed_seconds;
    const static int pos_array_size = 3;
    const static int rot_array_size = 4;
};
