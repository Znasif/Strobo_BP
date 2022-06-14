// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_DualCameraImagePlane.h"
#include "ViveSR_DualCameraImageCapture.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"
#include "Engine/World.h"
#include "GenericPlatform/GenericPlatformMisc.h"


// Sets default values
AViveSR_DualCameraImagePlane::AViveSR_DualCameraImagePlane()
{
    PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));

    FVector anchorLeft = FVector(10, -0.0325f, -0.04f);
    FVector anchorRight = FVector(10, 0.0325f, -0.04f);
	FVector anchorDepth = FVector(5.0f, 0.0f, 0.0f);
    DistortedImagePlaneLeft = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMeshLeft"));
    SetupImagePlaneMesh(DistortedImagePlaneLeft, anchorLeft * 100);
    DistortedImagePlaneRight = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMeshRight"));
    SetupImagePlaneMesh(DistortedImagePlaneRight, anchorRight * 100);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneVisualAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
    if (PlaneVisualAsset.Succeeded())
    {
        UndistortedImagePlaneLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ImagePlane (Undistorted Left)"));
        UndistortedImagePlaneLeft->SetStaticMesh(PlaneVisualAsset.Object);
        SetupImagePlaneMesh(UndistortedImagePlaneLeft, anchorLeft * M_TO_CM);
        UndistortedImagePlaneRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ImagePlane (Undistorted Right)"));
        UndistortedImagePlaneRight->SetStaticMesh(PlaneVisualAsset.Object);
        SetupImagePlaneMesh(UndistortedImagePlaneRight, anchorRight * M_TO_CM);

        CalibratedImagePlaneLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ImagePlane (Calibrated Left)"));
        CalibratedImagePlaneLeft->SetStaticMesh(PlaneVisualAsset.Object);
        SetupImagePlaneMesh(CalibratedImagePlaneLeft, anchorLeft * M_TO_CM);
        CalibratedImagePlaneRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ImagePlane (Calibrated Right)"));
        CalibratedImagePlaneRight->SetStaticMesh(PlaneVisualAsset.Object);
        SetupImagePlaneMesh(CalibratedImagePlaneRight, anchorRight * M_TO_CM);

        DepthImagePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ImagePlane (Depth)"));
        DepthImagePlane->SetStaticMesh(PlaneVisualAsset.Object);
        SetupImagePlaneMesh(DepthImagePlane, anchorDepth * M_TO_CM);
    }
    else
    {
        UE_LOG(LogViveSR, Warning, TEXT("Image plane mesh not found."));
    }
}

void AViveSR_DualCameraImagePlane::SetImgPlaneOwner(AActor* actor) {
    if (this) {
        this->SetOwner(actor);
        UndistortedImagePlaneLeft->SetOwnerNoSee(true);
        UndistortedImagePlaneRight->SetOwnerNoSee(true);
        DepthImagePlane->SetOwnerNoSee(true);
        CalibratedImagePlaneLeft->SetOnlyOwnerSee(true);
        CalibratedImagePlaneRight->SetOnlyOwnerSee(true);
    }
}

// Called when the game starts or when spawned
void AViveSR_DualCameraImagePlane::BeginPlay()
{
    Super::BeginPlay();

}

void AViveSR_DualCameraImagePlane::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}


void AViveSR_DualCameraImagePlane::Initial()
{
    distorted_img_plane_width = ViveSR_DualCameraImageCapture::Instance()->distorted_img_width;
    distorted_img_plane_height = ViveSR_DualCameraImageCapture::Instance()->distorted_img_height;
    undistorted_img_plane_width = ViveSR_DualCameraImageCapture::Instance()->undistorted_img_width;
    undistorted_img_plane_height = ViveSR_DualCameraImageCapture::Instance()->undistorted_img_height;
    depth_img_plane_width = ViveSR_DualCameraImageCapture::Instance()->depth_img_width;
    depth_img_plane_height = ViveSR_DualCameraImageCapture::Instance()->depth_img_height;

    camera_focal_length = ViveSR_DualCameraImageCapture::Instance()->focal_length_right;
    undistortedCx[(int)DualCameraIndex::LEFT] = ViveSR_DualCameraImageCapture::Instance()->undistorted_cx_left;
    undistortedCx[(int)DualCameraIndex::RIGHT] = ViveSR_DualCameraImageCapture::Instance()->undistorted_cx_right;
    undistortedCy[(int)DualCameraIndex::LEFT] = ViveSR_DualCameraImageCapture::Instance()->undistorted_cy_left;
    undistortedCy[(int)DualCameraIndex::RIGHT] = ViveSR_DualCameraImageCapture::Instance()->undistorted_cy_right;

    FVector anchorLeft = ViveSR_DualCameraImageCapture::Instance()->img4K_ready ? FVector(10, -0.0325f, -0.05f) : FVector(10, -0.0325f, 0.0f);
    FVector anchorRight = ViveSR_DualCameraImageCapture::Instance()->img4K_ready ? FVector(10, 0.0325f, -0.05f) : FVector(10, 0.0325f, 0.0f);
    SetupImagePlaneMesh(UndistortedImagePlaneLeft, anchorLeft * M_TO_CM);
    SetupImagePlaneMesh(UndistortedImagePlaneRight, anchorRight * M_TO_CM);
    SetupImagePlaneMesh(CalibratedImagePlaneLeft, anchorLeft * M_TO_CM);
    SetupImagePlaneMesh(CalibratedImagePlaneRight, anchorRight * M_TO_CM);

    float plane_resize_factor = 0.83f;
    float width_height_ratio = (float(undistorted_img_plane_width) / float(undistorted_img_plane_height));
    int plane_distance = 10;
    float depth_plane_resize_factor = 0.25f;
    int depth_plane_distance = 5;
    UndistortedImagePlaneLeft->SetRelativeScale3D(FVector(plane_distance * (float)undistorted_img_plane_width / camera_focal_length, plane_distance * (float)undistorted_img_plane_height / camera_focal_length, 1.0f));
    UndistortedImagePlaneRight->SetRelativeScale3D(FVector(plane_distance * (float)undistorted_img_plane_width / camera_focal_length, plane_distance * (float)undistorted_img_plane_height / camera_focal_length, 1.0f));
    CalibratedImagePlaneLeft->SetRelativeScale3D(FVector(plane_distance * (float)undistorted_img_plane_width / CalculateFocalLength(SCENECAPTURE_FOV, undistorted_img_plane_width)*plane_resize_factor, plane_distance * (float)undistorted_img_plane_height / CalculateFocalLength(SCENECAPTURE_FOV, undistorted_img_plane_width)*plane_resize_factor*width_height_ratio, 1.0f));
    CalibratedImagePlaneRight->SetRelativeScale3D(FVector(plane_distance * (float)undistorted_img_plane_width / CalculateFocalLength(SCENECAPTURE_FOV, undistorted_img_plane_width)*plane_resize_factor, plane_distance * (float)undistorted_img_plane_height / CalculateFocalLength(SCENECAPTURE_FOV, undistorted_img_plane_width)*plane_resize_factor*width_height_ratio, 1.0f));
    DepthImagePlane->SetRelativeScale3D(FVector(depth_plane_distance * (float)depth_img_plane_width / CalculateFocalLength(SCENECAPTURE_FOV, depth_img_plane_width)*depth_plane_resize_factor, depth_plane_distance * (float)depth_img_plane_height / CalculateFocalLength(SCENECAPTURE_FOV, depth_img_plane_width)*depth_plane_resize_factor, 1.0f));
}

float AViveSR_DualCameraImagePlane::CalculateFocalLength(float FoV, int img_width) {
    return 0.5f*img_width / FMath::Atan(FoV*0.5f);
}

void AViveSR_DualCameraImagePlane::SetMode(DualCameraDisplayMode mode)
{
    switch (mode)
    {
    case DualCameraDisplayMode::MIX:
#ifdef USE_UNDISTORT_MESH
        DistortedImagePlaneLeft->SetHiddenInGame(false);
        DistortedImagePlaneRight->SetHiddenInGame(false);
#else
        UndistortedImagePlaneLeft->SetHiddenInGame(false);
        UndistortedImagePlaneRight->SetHiddenInGame(false);
        CalibratedImagePlaneLeft->SetHiddenInGame(false);
        CalibratedImagePlaneRight->SetHiddenInGame(false);
        SetMeshComponentNoSeeInVRCamera();
#endif
        SetActorTickEnabled(true);
        break;
    case DualCameraDisplayMode::VIRTUAL:
    default:
#ifdef USE_UNDISTORT_MESH
        DistortedImagePlaneLeft->SetHiddenInGame(true);
        DistortedImagePlaneRight->SetHiddenInGame(true);
#else
        UndistortedImagePlaneLeft->SetHiddenInGame(true);
        UndistortedImagePlaneRight->SetHiddenInGame(true);
        CalibratedImagePlaneLeft->SetHiddenInGame(true);
        CalibratedImagePlaneRight->SetHiddenInGame(true);
        SetMeshComponentOnlySeeInVRCamera();
#endif
        SetActorTickEnabled(false);
        break;
    }
}


void AViveSR_DualCameraImagePlane::SetMeshComponentNoSeeInVRCamera() {
    auto pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), foundActors);
    if (!pawn) return;
    if (foundActors.Num() == 0) return;
    for (auto &actor : foundActors) {
        if (actor->GetClass() == AViveSR_DualCameraImagePlane::StaticClass()) {
            AViveSR_DualCameraImagePlane* planeActor = (AViveSR_DualCameraImagePlane*)actor;
            planeActor->SetImgPlaneOwner(static_cast<AActor*> (pawn));
            continue;
        }
        if (actor != pawn) actor->SetOwner(static_cast<AActor*> (pawn));
        TArray<UTextRenderComponent*> textcomps;
        actor->GetComponents<UTextRenderComponent>(textcomps);
        if (textcomps.Num() != 0) {
            for (auto &mesh : textcomps) {
                mesh->SetOwnerNoSee(true);
                mesh->SetOnlyOwnerSee(false);
            }
        }
        TArray<UMeshComponent*> widgetcomps;
        actor->GetComponents<UMeshComponent>(widgetcomps);
        if (widgetcomps.Num() == 0) continue;
        for (auto &mesh : widgetcomps) {
            mesh->SetOwnerNoSee(true);
            mesh->SetOnlyOwnerSee(false);
        }        
    }
}
void AViveSR_DualCameraImagePlane::SetMeshComponentOnlySeeInVRCamera() {
    auto pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!pawn) return;
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), foundActors);
    if (foundActors.Num() == 0) return;
    for (auto &actor : foundActors) {
        if (actor->GetClass() == AViveSR_DualCameraImagePlane::StaticClass()) continue;
        if (actor != pawn) actor->SetOwner(static_cast<AActor*> (pawn));
        TArray<UTextRenderComponent*> textcomps;
        actor->GetComponents<UTextRenderComponent>(textcomps);
        if (textcomps.Num() != 0) {
            for (auto &mesh : textcomps) {
                mesh->SetOnlyOwnerSee(true);
                mesh->SetOwnerNoSee(false);
            }
        }
        TArray<UMeshComponent*> widgetcomps;
        actor->GetComponents<UMeshComponent>(widgetcomps);
        if (widgetcomps.Num() == 0) continue;
        for (auto &mesh : widgetcomps) {
            mesh->SetOnlyOwnerSee(true);
            mesh->SetOwnerNoSee(false);
        }
    }
}

