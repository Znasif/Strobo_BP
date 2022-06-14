// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_RigidReconstructionRender.h"
#include "ViveSR_URigidReconstruction.h"
#include "ViveSR_SceneUnderstanding.h"
#include "ViveSR_RigidReconstructionEnums.h"
#include "ViveSR_Enums.h"
#include "SRWorks.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include <algorithm>

// Sets default values
AViveSR_RigidReconstructionRender::AViveSR_RigidReconstructionRender()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;
    MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
    RootComponent = MeshComponent;
    MeshComponent->bUseAsyncCooking = true;
}

// Called when the game starts or when spawned
void AViveSR_RigidReconstructionRender::BeginPlay()
{
    Super::BeginPlay();
    SetActorTickInterval(0.022F);
}

void AViveSR_RigidReconstructionRender::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (ViveSR_URigidReconstruction::Instance()->is_scanning)
        ViveSR_URigidReconstruction::Instance()->StopScanning();
}

// Called every frame
void AViveSR_RigidReconstructionRender::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SetSceneUnderstandingInfo();
    Display();
    if (!ViveSR_URigidReconstruction::Instance()->is_scanning) {
        FlushPersistentDebugLines(GetWorld());
    }
    if (EnablePreviewScanModel) {
        MeshComponent->CreateMeshSection_LinearColor(0, vertex_preview, index_preview, normals, UV0, vertex_color_preview, tangents, true);
    }
}

void AViveSR_RigidReconstructionRender::Initial()
{
    ViveSR_URigidReconstruction::Instance()->Initial();
    ViveSR_URigidReconstruction::Instance()->AllocOutputDataMemory();

    SetMeshDisplayMode(DisplayMode);
    if (MeshMaterial != nullptr) SetMeshMaterial(MeshMaterial);

    
    if (DisplayMode == ReconstructionDisplayMode::ADAPTIVE_MESH) {
        adaptive_range_min = FMath::Pow(2, (int)MinGridSize + 1);
        adaptive_range_max = FMath::Pow(2, (int)MaxGridSize + 1);
        if (adaptive_range_min <= adaptive_range_max) {
            ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterFloat(ViveSR::RigidReconstruction::Param::ADAPTIVE_MAX_GRID, adaptive_range_max* 0.01f);
            ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterFloat(ViveSR::RigidReconstruction::Param::ADAPTIVE_MIN_GRID, adaptive_range_min* 0.01f);
            ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterFloat(ViveSR::RigidReconstruction::Param::ADAPTIVE_ERROR_THRES, ErrorThres* 0.01f);
        }
    }
    SetActorTickEnabled(true);
}
void AViveSR_RigidReconstructionRender::SetSceneUnderstandingInfo() {
    if (last_enabled_sceneunderstanding == EnabledSceneUnderstanding) return;
    int result = ViveSR_SceneUnderstanding::Instance()->EnableSceneUnderstanding(EnabledSceneUnderstanding);
    if (EnabledSceneUnderstanding) {
        TArray<bool> isObjectEnable{ EnableFloor, EnableWall, EnableCeiling, EnableChair, EnableTable, EnableBed,EnableMonitor,
                                     EnableWindow, EnableFurniture, EnableDoor, EnablePicture,
                                     EnablePerson, EnableLight, EnablePlant, EnableCurtain, EnablePillow };
        int object = ViveSR::RigidReconstruction::SceneUnderstandingObjectType::FLOOR;
        for (auto const& isEnable : isObjectEnable) {
            ViveSR_SceneUnderstanding::Instance()->SetCustomSceneUnderstandingConfig((ViveSR::RigidReconstruction::SceneUnderstandingObjectType)object, 10, isEnable);
            ++object;
        }
    }
    last_enabled_sceneunderstanding = EnabledSceneUnderstanding;
}
void AViveSR_RigidReconstructionRender::Release() {
    ViveSR_URigidReconstruction::Instance()->is_scanning = false;
    ViveSR_URigidReconstruction::Instance()->Release();
}

bool AViveSR_RigidReconstructionRender::SetMeshDisplayMode(ReconstructionDisplayMode displayMode)
{
    int result = ViveSR::Error::FAILED;
    if (displayMode == ReconstructionDisplayMode::ADAPTIVE_MESH)
    {
        result = ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Param::LITE_POINT_CLOUD_MODE, false);
        result = ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Param::FULL_POINT_CLOUD_MODE, false);
        result = ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Param::LIVE_ADAPTIVE_MODE, true);
    }

    else if (displayMode == ReconstructionDisplayMode::FIELD_OF_VIEW)
    {
        result = ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Param::LITE_POINT_CLOUD_MODE, true);
        result = ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Param::FULL_POINT_CLOUD_MODE, false);
        result = ViveSR::SRWork::RigidReconstruction::SetReconstructionParameterBool(ViveSR::RigidReconstruction::Param::LIVE_ADAPTIVE_MODE, false);
    }
    return (result == ViveSR::Error::WORK);
}

void AViveSR_RigidReconstructionRender::SetMeshMaterial(UMaterialInterface *material)
{

    MeshComponent->SetMaterial(0, material);
}
bool AViveSR_RigidReconstructionRender::GetisSceneUnderstanding() {
    return EnabledSceneUnderstanding;
}

void AViveSR_RigidReconstructionRender::Display() {

    if (ViveSR_URigidReconstruction::Instance()->is_scanning)
    {
        ViveSR_URigidReconstruction::Instance()->GetRigidReconstructionFrame(current_frame_id);
        if (current_frame_id != last_processed_frame)
        {
            ViveSR_URigidReconstruction::Instance()->GetRigidReconstructionData(
                current_frame_id,
                pose,
                vertex_count,
                vertex_data,
                vertex_in_float,
                index_count,
                index_data);
            if (vertex_count > 0)
            {

                valid_vertex_data.Init(FVector::ZeroVector, vertex_count);
                point_cloud_color.Init(FColor::White, vertex_count);
                TArray<FLinearColor> vertex_color;
                FLinearColor linear_color;
                color_set = new uint8[4];
                for (int i = 0; i < vertex_count; i++)
                {
                    float x = vertex_data[i * 4 + 2];
                    float y = vertex_data[i * 4 + 0];
                    float z = vertex_data[i * 4 + 1];
                    valid_vertex_data[i].Set(x * 100, y * 100, z * 100);
                    FColor color;
                    memcpy(color_set, (vertex_data.GetData() + i * 4 + 3), 4);
                    color.R = color_set[0];
                    color.G = color_set[1];
                    color.B = color_set[2];
                    color.A = color_set[3];
                    if (DisplayMode == ReconstructionDisplayMode::FIELD_OF_VIEW) {
                        point_cloud_color[i] = color;
                    }
                    if (EnabledSceneUnderstanding) {
                        linear_color = FLinearColor(color);
                    }
                    else {
                        linear_color = FLinearColor(0, 0.2357, 0.943, 0);
                    }
                    vertex_color.Add(linear_color);
                }
                delete[] color_set;
                color_set = nullptr;
                if (DisplayMode == ReconstructionDisplayMode::ADAPTIVE_MESH) {
                    TArray<int> valid_index_data;
                    valid_index_data.Init(0, index_count);
                    for (int i = 0; i < index_count; i += 3)
                    {
                        valid_index_data[i + 0] = index_data[i + 0];
                        valid_index_data[i + 1] = index_data[i + 2];
                        valid_index_data[i + 2] = index_data[i + 1];
                    }
                    MeshComponent->CreateMeshSection_LinearColor(0, valid_vertex_data, valid_index_data, normals, UV0, vertex_color, tangents, true);
                    index_preview.Init(0, index_count);
                    std::copy(valid_index_data.GetData(), valid_index_data.GetData() + valid_index_data.Num(), index_preview.GetData());
                }
                vertex_preview.Init(FVector::ZeroVector, vertex_count);
                std::copy(valid_vertex_data.GetData(), valid_vertex_data.GetData() + valid_vertex_data.Num(), vertex_preview.GetData());
                vertex_color_preview.Init(FLinearColor::Blue, vertex_count);
                std::copy(vertex_color.GetData(), vertex_color.GetData() + vertex_color.Num(), vertex_color_preview.GetData());
            }
            last_processed_frame = current_frame_id;
        }
        if (DisplayMode == ReconstructionDisplayMode::FIELD_OF_VIEW) {
            for (int k = 0; k < valid_vertex_data.Num(); k++) {
                DrawDebugPoint(GetWorld(), valid_vertex_data[k], 22, point_cloud_color[k], false, 0.044F, 0);
            }
        }
    }
    if (ViveSR_URigidReconstruction::Instance()->is_exporting) {
        ViveSR_URigidReconstruction::Instance()->UpdateExportProgress();
    }
    if (ViveSR_SceneUnderstanding::Instance()->is_exporting) {
        ViveSR_SceneUnderstanding::Instance()->UpdateSceneUnderstandingProgress();
    }
}