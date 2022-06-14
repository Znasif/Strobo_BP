// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_DualCameraDepthCollider.h"
#include "ViveSR_DualCameraDepthExtra.h"
#include "SRWork_Client_API.h"
#include "ViveSR_PassThrough.h"
#include "ViveSR_Depth.h"
#include "SRWorks.h"
#include "ViveSR_Enums.h"
#include "ViveSR_DepthEnums.h"
// Sets default values
AViveSR_DualCameraDepthCollider::AViveSR_DualCameraDepthCollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("DepthCollider"));
	RootComponent = MeshComponent;
	MeshComponent->bUseAsyncCooking = true;
    
}

// Called when the game starts or when spawned
void AViveSR_DualCameraDepthCollider::BeginPlay()
{
	Super::BeginPlay();
    if (MeshMaterial != nullptr)
    MeshComponent->SetMaterial(0, MeshMaterial);
}

// Called every frame
void AViveSR_DualCameraDepthCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (LastUpdateDepthCollider != UpdateDepthCollider)
        EnableDepthCollider(UpdateDepthCollider);
	if (UpdateDepthCollider)
	{
		ViveSR_DualCameraDepthExtra::Instance()->UpdateDepthMeshData();
		ViveSR_DualCameraDepthExtra::Instance()->GetDepthColliderFrameInfo();
		int currentDepthColliderUpdateTime = ViveSR_DualCameraDepthExtra::Instance()->time_index;
		if (currentDepthColliderUpdateTime != last_update_time)
		{
			ExtractCurrentColliders();
			last_update_time = currentDepthColliderUpdateTime;
		}
	}
    LastUpdateDepthCollider = UpdateDepthCollider;
}
void AViveSR_DualCameraDepthCollider::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}
void AViveSR_DualCameraDepthCollider::Release() {
	UpdateDepthCollider = false;
	ViveSR_DualCameraDepthExtra::Instance()->Release();
}

void AViveSR_DualCameraDepthCollider::ExtractCurrentColliders()
{
	ViveSR_DualCameraDepthExtra::Instance()->UpdateDepthMeshData();
	ViveSR_DualCameraDepthExtra::Instance()->GetDepthColliderData(vertex_data_num, vertex_data, idx_data_num, idx_data);
	if (vertex_data_num > 0 && idx_data_num > 0)
	{
		GenerateMeshColliders();
	}
}

void AViveSR_DualCameraDepthCollider::GenerateMeshColliders()
{
	TArray<FVector> valid_vertex_data;
	valid_vertex_data.Init(FVector::ZeroVector, vertex_data_num);
	for (int i = 0; i < vertex_data_num; ++i)
	{
		float x = vertex_data[i * 3 + 2];
		float y = vertex_data[i * 3 + 0];
		float z = vertex_data[i * 3 + 1];
		valid_vertex_data[i].Set(x * 100, y * 100, z * 100);
	}

	TArray<int> valid_idx_data;
	valid_idx_data.Init(0, idx_data_num);
	for (int i = 0; i < idx_data_num; i += 3)
	{
		valid_idx_data[i + 0] = idx_data[i + 0];
		valid_idx_data[i + 1] = idx_data[i + 2];
		valid_idx_data[i + 2] = idx_data[i + 1];
	}

	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertex_color;

	MeshComponent->CreateMeshSection_LinearColor(0, valid_vertex_data, valid_idx_data, normals, UV0, vertex_color, tangents, true);

	// Enable collision data
	MeshComponent->ContainsPhysicsTriMeshData(true);
}

int AViveSR_DualCameraDepthCollider::EnableDepthCollider(bool enable)
{
    int result = ViveSR::Error::FAILED;

    if (enable)
    {
        result = ViveSR::SRWork::PassThrough::SetSkipVGAProcess(false);
        result = ViveSR::SRWork::LinkModule(ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE, ViveSR::SRWork::DepthMesh::MODULE_DEPTH_MESH_TYPE);
    }
    else
    {
        result = ViveSR::SRWork::UnlinkModule(ViveSR::SRWork::Depth::MODULE_DEPTH_TYPE, ViveSR::SRWork::DepthMesh::MODULE_DEPTH_MESH_TYPE);
    }
    result = ViveSR::SRWork::DepthMesh::SetDepthMeshIsEnable(enable);
    UpdateDepthCollider = enable;
    return result;
}
void AViveSR_DualCameraDepthCollider::SetLiveMeshVisibility(bool visible)
{
	MeshComponent->SetVisibility(visible);
	DepthColliderVisibility = visible;
}

bool AViveSR_DualCameraDepthCollider::GetQualityScale(int &value)
{
	int result = ViveSR::SRWork::Depth::GetDepthParameterInt( ViveSR::Depth::Param::COLLIDER_QUALITY, &value);
	return result == ViveSR::Error::WORK;
}

bool AViveSR_DualCameraDepthCollider::SetQualityScale(int value)
{
	int result = ViveSR::SRWork::Depth::SetDepthParameterInt( ViveSR::Depth::Param::COLLIDER_QUALITY, value);
	if (result == ViveSR::Error::WORK)
	{
		quality_scale = value;
		return true;
	}
	else
		return false;
}