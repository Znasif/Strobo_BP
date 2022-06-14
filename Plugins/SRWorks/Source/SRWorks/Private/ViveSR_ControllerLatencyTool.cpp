// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_ControllerLatencyTool.h"
#include "Math/UnrealMathUtility.h"
#include "ViveSR_DualCameraImageCapture.h"
#include <algorithm>

ViveSR_ControllerLatencyTool* ViveSR_ControllerLatencyTool::Mgr = nullptr;
ViveSR_ControllerLatencyTool::ViveSR_ControllerLatencyTool()
{

}
ViveSR_ControllerLatencyTool::~ViveSR_ControllerLatencyTool()
{
}
ViveSR_ControllerLatencyTool* ViveSR_ControllerLatencyTool::Instance()
{
	if (Mgr == nullptr)
	{
		Mgr = new ViveSR_ControllerLatencyTool();
	}
	return Mgr;
}
void ViveSR_ControllerLatencyTool::Initial() {
    pose_info = new  ViveSR::SRWork::Controller::ControllerPoseData();
    pose_info->pose_position_left = new float[3];
    pose_info->pose_position_right = new float[3];
    pose_info->pose_rotation_left = new float[4];
    pose_info->pose_rotation_right = new float[4];
}
void ViveSR_ControllerLatencyTool::Release() {
    if (pose_info) {
        if (pose_info->pose_position_left) {
            delete[] pose_info->pose_position_left; pose_info->pose_position_left = nullptr;
        }
        if (pose_info->pose_position_right) {
            delete[] pose_info->pose_position_right; pose_info->pose_position_right = nullptr;
        }
        if (pose_info->pose_rotation_left) {
            delete[] pose_info->pose_rotation_left; pose_info->pose_rotation_left = nullptr;
        }
        if (pose_info->pose_rotation_right) {
            delete[] pose_info->pose_rotation_right; pose_info->pose_rotation_right = nullptr;
        }

        delete[] pose_info; pose_info = nullptr;
    }
    latency_map.Empty();
}
int ViveSR_ControllerLatencyTool::GetControllerTrackingPose() {
    int result = ViveSR::SRWork::Controller::GetControllerPoseData(pose_info);
    return result;
}
void ViveSR_ControllerLatencyTool::SetStartTime(std::chrono::time_point<std::chrono::steady_clock> startSrc) {
    start = startSrc;
}
void ViveSR_ControllerLatencyTool::AddPoseToMap(FVector position, FQuat rot) {
    PoseWithTimestamp current_pose_data;
    current = std::chrono::steady_clock::now();
    elapsed_seconds =  current - start;
    current_pose_data.time_stamp = elapsed_seconds.count();
    current_pose_data.rotation = rot.GetNormalized();
    current_pose_data.position = position;

    latency_map.Add(current_pose_data);
}
PoseWithTimestamp ViveSR_ControllerLatencyTool::GetLatencyPosition(float timeDelay)
{
    PoseWithTimestamp pose;

    //Use latest first
    pose.position = latency_map[latency_map.Num() - 1].position;
    pose.rotation = latency_map[latency_map.Num() - 1].rotation;

    for (int i = 0; i < latency_map.Num(); ++i)
    {
        // Find first one that newer than target time
        if (latency_map[i].time_stamp > FMath::Abs(elapsed_seconds.count() - timeDelay))
        {
            if (i > 0)
            {
                //Linearly interpolates between two poses
                float delta = latency_map[i].time_stamp - latency_map[i - 1].time_stamp;
                float alpha = ((elapsed_seconds.count() - latency_map[i - 1].time_stamp) - timeDelay) / delta;

                FVector pos =  FMath::Lerp(latency_map[i - 1].position, latency_map[i].position, alpha);
                FQuat rot = FMath::Lerp(latency_map[i - 1].rotation, latency_map[i].rotation, alpha);

                pose.position = pos;
                pose.rotation = rot;

                latency_map.RemoveAt(0, i - 1);
            }
            return pose;
        }
    }
    return pose;
}
