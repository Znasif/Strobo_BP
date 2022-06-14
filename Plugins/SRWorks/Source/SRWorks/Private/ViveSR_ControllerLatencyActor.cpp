// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_ControllerLatencyActor.h"

// Sets default values
AViveSR_ControllerLatencyActor::AViveSR_ControllerLatencyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void AViveSR_ControllerLatencyActor::BeginPlay()
{
	Super::BeginPlay();
    Initial();
    SetActorTickEnabled(true);
}

// Called every frame
void AViveSR_ControllerLatencyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    UpdateControllerLocation();
    data_instance.LocalPosition = FVector::ZeroVector;
    data_instance.Position = FVector::ZeroVector;
    data_instance.Rotation = FRotator::ZeroRotator;
    ObjectToMoveDataInfo.Init(data_instance, object_to_move.Num());
    for (int i = 0; i < object_to_move.Num(); i++) {
        ObjectToMoveDataInfo[i].LocalPosition = 100 * object_to_move[i].local_position;
        ObjectToMoveDataInfo[i].Position = 100 * object_to_move[i].position;
        ObjectToMoveDataInfo[i].Rotation = FRotator(object_to_move[i].rotation);
    }   
}

void AViveSR_ControllerLatencyActor::Initial() {
    ViveSR_ControllerLatencyTool::Instance()->Initial();
    start = std::chrono::steady_clock::now();
    ViveSR_ControllerLatencyTool::Instance()->SetStartTime(start);
   
}

void AViveSR_ControllerLatencyActor::Release() {
	
	ViveSR_ControllerLatencyTool::Instance()->Release();
    object_to_move.Empty();
    ObjectToMoveDataInfo.Empty();
}
void AViveSR_ControllerLatencyActor::AddObjectToMove(FVector position, FRotator rotation, FVector localPosition, FVector pos, FVector rot) {
    ObjectToMoveData data;
    data.position = position;
    data.rotation = rotation.Quaternion();
    data.local_position = localPosition;
    data.pos = pos;
    data.rot = rot;

    object_to_move.Add(data);
}
void AViveSR_ControllerLatencyActor::UpdateControllerLocation() {
    ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_is_valid_left = 0;
    ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_is_valid_right = 0;

    int result = ViveSR_ControllerLatencyTool::Instance()->GetControllerTrackingPose();

    if (result != (int)ViveSR::Error::WORK) return;
    if (TrackController == ControllerHandType::LEFT_HAND)
    {
        if (ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_is_valid_left != 0)
            // Convert to Ue4 coordinate with index 2,0,1
            ViveSR_ControllerLatencyTool::Instance()->AddPoseToMap(FVector(ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_position_left[2], ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_position_left[0], ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_position_left[1]),
                FQuat(ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_rotation_left[2], ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_rotation_left[0], ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_rotation_left[1], ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_rotation_left[3]));
    }
    else if (TrackController == ControllerHandType::RITHT_HAND)
    {
        if (ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_is_valid_right != 0)
            // Convert to Ue4 coordinate with index 2,0,1
            ViveSR_ControllerLatencyTool::Instance()->AddPoseToMap(FVector(ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_position_right[2], ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_position_right[0], ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_position_right[1]),
                FQuat(ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_rotation_right[2], ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_rotation_right[0], ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_rotation_right[1], ViveSR_ControllerLatencyTool::Instance()->pose_info->pose_rotation_right[3]));
    }
    

    if (LatencyCompensation > 0 && ViveSR_ControllerLatencyTool::Instance()->latency_map.Num() > 0)
    {
        PoseWithTimestamp pose = ViveSR_ControllerLatencyTool::Instance()->GetLatencyPosition((float)(LatencyCompensation/1000.0f));

        if (MoveAttachedObject)
        {
            SetActorLocationAndRotation(pose.position, pose.rotation);
        }
        else if (object_to_move.Num() > 0)
        {
            for (int i = 0; i < object_to_move.Num(); ++i)
            {
                object_to_move[i].position = pose.position;
                object_to_move[i].rotation = pose.rotation;

                object_to_move[i].local_position += object_to_move[i].pos;
                object_to_move[i].rotation *= FQuat::MakeFromEuler(object_to_move[i].rot);
            }
        }
    }
   
}
void AViveSR_ControllerLatencyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    Release();
}