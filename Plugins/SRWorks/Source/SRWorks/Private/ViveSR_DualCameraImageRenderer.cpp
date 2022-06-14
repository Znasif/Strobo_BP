// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_DualCameraImageRenderer.h"
#include "ViveSR.h"
#include "ViveSR_DualCameraImageCapture.h"
#include "ViveSR_DualCameraImagePlane.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"


AViveSR_DualCameraImageRenderer::AViveSR_DualCameraImageRenderer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

    CalibratedCameraLeft = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureLeft"));
    CalibratedCameraRight = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureRight"));
    CalibratedCameraLeft->FOVAngle = 107;
    CalibratedCameraRight->FOVAngle = 107;
    CalibratedCameraLeft->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
    CalibratedCameraRight->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> distortedMatInstAssetLeft(TEXT("MaterialInstanceConstant'/SRWorks/Materials/DistortedMaterialLeft_Inst.DistortedMaterialLeft_Inst'"));
	if (distortedMatInstAssetLeft.Succeeded())
	{
		DistortedLeftCameraImageMaterialInstance = distortedMatInstAssetLeft.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> distortedMatInstAssetRight(TEXT("MaterialInstanceConstant'/SRWorks/Materials/DistortedMaterialRight_Inst.DistortedMaterialRight_Inst'"));
	if (distortedMatInstAssetRight.Succeeded())
	{
		DistortedRightCameraImageMaterialInstance = distortedMatInstAssetRight.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> undistortedMatInstAssetLeft(TEXT("MaterialInstanceConstant'/SRWorks/Materials/UndistortedMaterialLeft_Inst.UndistortedMaterialLeft_Inst'"));
	if (undistortedMatInstAssetLeft.Succeeded())
	{
		UndistortedLeftCameraImageMaterialInstance = undistortedMatInstAssetLeft.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> undistortedMatInstAssetRight(TEXT("MaterialInstanceConstant'/SRWorks/Materials/UndistortedMaterialRight_Inst.UndistortedMaterialRight_Inst'"));
	if (undistortedMatInstAssetRight.Succeeded())
	{
		UndistortedRightCameraImageMaterialInstance = undistortedMatInstAssetRight.Object;
	}
    static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> calibratedMatInstAssetLeft(TEXT("MaterialInstanceConstant'/SRWorks/Materials/CalibratedMaterialLeft_Inst.CalibratedMaterialLeft_Inst'"));
    if (calibratedMatInstAssetLeft.Succeeded())
    {
        CalibrationLeftImageMaterialInstance = calibratedMatInstAssetLeft.Object;
    }
    static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> calibratedMatInstAssetRight(TEXT("MaterialInstanceConstant'/SRWorks/Materials/CalibratedMaterialRight_Inst.CalibratedMaterialRight_Inst'"));
    if (calibratedMatInstAssetRight.Succeeded())
    {
        CalibrationRightImageMaterialInstance = calibratedMatInstAssetRight.Object;
    }
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> DePurpleMatInstAssetLeft(TEXT("MaterialInstanceConstant'/SRWorks/Materials/dePurple_Mat_Inst_Left.dePurple_Mat_Inst_Left'"));
	if (DePurpleMatInstAssetLeft.Succeeded())
	{
		DePurpleLeftMaterialInstance = DePurpleMatInstAssetLeft.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> DePurpleMatInstAssetRight(TEXT("MaterialInstanceConstant'/SRWorks/Materials/dePurple_Mat_Inst_Right.dePurple_Mat_Inst_Right'"));
	if (DePurpleMatInstAssetRight.Succeeded())
	{
		DePurpleRightMaterialInstance = DePurpleMatInstAssetRight.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> depthMatInstAsset(TEXT("MaterialInstanceConstant'/SRWorks/Materials/DepthMaterial_Inst.DepthMaterial_Inst'"));
	if (depthMatInstAsset.Succeeded())
	{
		DepthMaterialInstance = depthMatInstAsset.Object;
	}
	else
	{
		UE_LOG(LogViveSR, Warning, TEXT("Default Depth Material Instance not found."));
	}
    
    static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> CalibratedRenderTargetLeftAsset(TEXT("Material'/SRWorks/Materials/CalibratedRenderTarget2DLeft.CalibratedRenderTarget2DLeft'"));
    if (CalibratedRenderTargetLeftAsset.Succeeded())
    {
        CalibratedRenderTargetLeft = CalibratedRenderTargetLeftAsset.Object;
        CalibratedRenderTargetLeft->AddToRoot();
    }
    else
    {
        UE_LOG(LogViveSR, Warning, TEXT("Default Calibrated Render Target Left not found."));
    }
    static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> CalibratedRenderTargetRightAsset(TEXT("Material'/SRWorks/Materials/CalibratedRenderTarget2DRight.CalibratedRenderTarget2DRight'"));
    if (CalibratedRenderTargetRightAsset.Succeeded())
    {
        CalibratedRenderTargetRight = CalibratedRenderTargetRightAsset.Object;
        CalibratedRenderTargetRight->AddToRoot();
    }
  
    else
    {
        UE_LOG(LogViveSR, Warning, TEXT("Default Calibrated Render Target Right not found."));
    }
    
}

void AViveSR_DualCameraImageRenderer::SetMode(DualCameraDisplayMode mode)
{
	DisplayMode = mode;
	static_cast<AViveSR_DualCameraImagePlane*>(image_plane)->SetMode(mode);

	switch (mode)
	{
	case DualCameraDisplayMode::MIX:
		SetActorTickEnabled(true);
		break;
	case DualCameraDisplayMode::VIRTUAL:
	default:
		SetActorTickEnabled(false);
		break;
	}
}
void AViveSR_DualCameraImageRenderer::Initial()
{
    // Initial ViveSR_DualCameraImageCapture
    ViveSR_DualCameraImageCapture::Instance()->SetGPUMethod(GPUMethod);
    ViveSR_DualCameraImageCapture::Instance()->Initial();

    distorted_img_width = ViveSR_DualCameraImageCapture::Instance()->distorted_img_width;
    distorted_img_height = ViveSR_DualCameraImageCapture::Instance()->distorted_img_height;
    undistorted_img_width = ViveSR_DualCameraImageCapture::Instance()->undistorted_img_width;
    undistorted_img_height = ViveSR_DualCameraImageCapture::Instance()->undistorted_img_height;
    camera_status = DualCameraStatus::WORKING;

    DistortedLeftCameraImageMaterial = UMaterialInstanceDynamic::Create(DistortedLeftCameraImageMaterialInstance, this);
    DistortedRightCameraImageMaterial = UMaterialInstanceDynamic::Create(DistortedRightCameraImageMaterialInstance, this);
    UndistortedLeftCameraImageMaterial = UMaterialInstanceDynamic::Create(UndistortedLeftCameraImageMaterialInstance, this);
    UndistortedRightCameraImageMaterial = UMaterialInstanceDynamic::Create(UndistortedRightCameraImageMaterialInstance, this);
    CalibrationLeftImageMaterial = UMaterialInstanceDynamic::Create(CalibrationLeftImageMaterialInstance, this);
    CalibrationRightImageMaterial = UMaterialInstanceDynamic::Create(CalibrationRightImageMaterialInstance, this);
    DePurpleLeftMaterial = UMaterialInstanceDynamic::Create(DePurpleLeftMaterialInstance, this);
    DePurpleRightMaterial = UMaterialInstanceDynamic::Create(DePurpleRightMaterialInstance, this);
    DepthDynamicMaterialSelf = UMaterialInstanceDynamic::Create(DepthMaterialInstance, this);
    is_horus = ViveSR_DualCameraImageCapture::Instance()->IsHorus();
    need_depurple = is_horus && PurpleFringeRemoval;
   
    UndistortedLeftCameraImageMaterials.Add(need_depurple ? DePurpleLeftMaterial : UndistortedLeftCameraImageMaterial);
    UndistortedRightCameraImageMaterials.Add(need_depurple? DePurpleRightMaterial: UndistortedRightCameraImageMaterial);
    CalibrationLeftImageMaterials.Add(CalibrationLeftImageMaterial);
    CalibrationRightImageMaterials.Add(CalibrationRightImageMaterial);
    DistortedLeftCameraImageMaterials.Add(DistortedLeftCameraImageMaterial);
    DistortedRightCameraImageMaterials.Add(DistortedRightCameraImageMaterial);  
    DepthMaterials.Add(DepthDynamicMaterialSelf);
    UWorld* world = GetWorld();
    if (world) {
        AViveSR_DualCameraImagePlane* planeActor = world->SpawnActor<AViveSR_DualCameraImagePlane>();
        planeActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        planeActor->Initial();
        planeActor->DistortedImagePlaneLeft->SetMaterial(0, DistortedLeftCameraImageMaterial);
        planeActor->DistortedImagePlaneRight->SetMaterial(0, DistortedRightCameraImageMaterial);
        planeActor->UndistortedImagePlaneLeft->SetMaterial(0, UndistortedLeftCameraImageMaterials[ori_img_idx]);
        planeActor->UndistortedImagePlaneRight->SetMaterial(0, UndistortedRightCameraImageMaterials[ori_img_idx]);
        planeActor->CalibratedImagePlaneLeft->SetMaterial(0, CalibrationLeftImageMaterials[ori_img_idx]);
        planeActor->CalibratedImagePlaneRight->SetMaterial(0, CalibrationRightImageMaterials[ori_img_idx]);
        planeActor->DepthImagePlane->SetMaterial(0, DepthDynamicMaterialSelf);
        image_plane = static_cast<AActor*>(planeActor);
        SetMode(DisplayMode);
        UE_LOG(LogTemp, Warning, TEXT("[ViveSR] DualCameraImageRenderer Initial "));

        CalibratedCameraLeft->HideComponent(planeActor->UndistortedImagePlaneRight);
        CalibratedRenderTargetLeft->SizeX = CalibratedRenderTargetLeft->SizeY = undistorted_img_width;
        CalibratedCameraLeft->TextureTarget = CalibratedRenderTargetLeft;

        CalibratedCameraRight->HideComponent(planeActor->UndistortedImagePlaneLeft);
        CalibratedRenderTargetRight->SizeX = CalibratedRenderTargetRight->SizeY = undistorted_img_width;       
		CalibratedCameraRight->TextureTarget = CalibratedRenderTargetRight;
    }
	SetActorTickEnabled(true);
}
void AViveSR_DualCameraImageRenderer::Release() {
	SetActorTickEnabled(false);
	camera_status = DualCameraStatus::IDLE;
	ViveSR_DualCameraImageCapture::Instance()->Release();
}
// Called when the game starts or when spawned
void AViveSR_DualCameraImageRenderer::BeginPlay()
{
	Super::BeginPlay();
		
}

void AViveSR_DualCameraImageRenderer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    static_cast<AViveSR_DualCameraImagePlane*>(image_plane)->SetImgPlaneOwner(static_cast<AActor*>(this));
}
// Called every frame
void AViveSR_DualCameraImageRenderer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (camera_status != DualCameraStatus::WORKING) return;

    if (UpdateDistortedMaterial) {       
        if (!ViveSR_DualCameraImageCapture::Instance()->UpdateDistortedImage())
            return;        
        UpdatePassThroughImage(DistortedLeftCameraImageMaterials, DistortedRightCameraImageMaterials, need_depurple);
        UpdateDistortedPose();
    }
    if (UpdateUndistortedMaterial) {        
        if (!ViveSR_DualCameraImageCapture::Instance()->UpdateUndistortedImage())
            return;
        UpdatePassThroughImage(UndistortedLeftCameraImageMaterials, UndistortedRightCameraImageMaterials, need_depurple);
        UpdateUndistortedPose();
    }
    if (UpdateDepthMaterial) {
        UpdateDepthImage();
    }    
}

void AViveSR_DualCameraImageRenderer::UpdatePassThroughImage(TArray<UMaterialInstanceDynamic*> matListLeft, TArray<UMaterialInstanceDynamic*> matListRight, bool de_purple) {
    int frame_idx, time_idx;
    matListLeft == DistortedLeftCameraImageMaterials ?
        ViveSR_DualCameraImageCapture::Instance()->GetDistortedTexture(ImageLeft, ImageRight, frame_idx, time_idx) :
        ViveSR_DualCameraImageCapture::Instance()->GetUndistortedTexture(ImageLeft, ImageRight, frame_idx, time_idx);
    if (last_texture_update_time == time_idx) return;
    last_texture_update_time = time_idx;
    real_fps = last_texture_update_time * 0.001f;
    for (int i = 0; i < matListLeft.Num(); i++) {
        if (IsValid(matListLeft[i]))
        {
            matListLeft[i]->SetTextureParameterValue(TEXT("Left_Eye"), ImageLeft);
            if (need_depurple) {
                matListLeft[i]->SetScalarParameterValue(TEXT("Gamma"), ViveSR_DualCameraImageCapture::Instance()->GetGamma());
            }
        }
    }
    for (int i = 0; i < matListRight.Num(); i++) {
        if (IsValid(matListRight[i]))
        {
            matListRight[i]->SetTextureParameterValue(TEXT("Right_Eye"), ImageRight);
            if (need_depurple) {
                matListRight[i]->SetScalarParameterValue(TEXT("Gamma"), ViveSR_DualCameraImageCapture::Instance()->GetGamma());
            }
        }
    }
}

void AViveSR_DualCameraImageRenderer::UpdateDepthImage() {
    UTexture2D *img;
    int frame_idx, time_idx;
    if (!ViveSR_DualCameraImageCapture::Instance()->UpdateDepthImage()) return;
    ViveSR_DualCameraImageCapture::Instance()->GetDepthTexture(img, frame_idx, time_idx);
    if (last_depth_update_time != time_idx)
    {
        last_depth_update_time = time_idx;
        real_depth_fps = last_depth_update_time * 0.001f;
        for (int i = 0; i < DepthMaterials.Num(); i++) {
            if (IsValid(DepthMaterials[i]))
            {
                DepthMaterials[i]->SetTextureParameterValue(TEXT("Depth"), img);
            }
        }
    }   
}
void AViveSR_DualCameraImageRenderer::UpdateUndistortedPose()
{
    auto pose_position = ((ViveSR_DualCameraImageCapture::Instance()->GetUndistortedLocalPosition(DualCameraIndex::LEFT) +
        ViveSR_DualCameraImageCapture::Instance()->GetUndistortedLocalPosition(DualCameraIndex::RIGHT)) / 2);
    auto pose_rotation = ViveSR_DualCameraImageCapture::Instance()->GetUndistortedLocalRotation(DualCameraIndex::LEFT);


    if (IsValid(vr_origin))
    {
        SetActorTransform(vr_origin->GetActorTransform());
        RootComponent->AddLocalTransform(FTransform(pose_rotation, pose_position * M_TO_CM));
        CalibratedCameraLeft->SetWorldLocationAndRotation(ViveSR_DualCameraImageCapture::Instance()->GetUndistortedLocalPosition(DualCameraIndex::LEFT) * M_TO_CM, pose_rotation);
        CalibratedCameraRight->SetWorldLocationAndRotation(ViveSR_DualCameraImageCapture::Instance()->GetUndistortedLocalPosition(DualCameraIndex::RIGHT) * M_TO_CM, pose_rotation);
    }
    else
    {
        RootComponent->SetWorldLocationAndRotation(pose_position * M_TO_CM, pose_rotation);
        CalibratedCameraLeft->SetWorldLocationAndRotation(ViveSR_DualCameraImageCapture::Instance()->GetUndistortedLocalPosition(DualCameraIndex::LEFT) * M_TO_CM, pose_rotation);
        CalibratedCameraRight->SetWorldLocationAndRotation(ViveSR_DualCameraImageCapture::Instance()->GetUndistortedLocalPosition(DualCameraIndex::RIGHT) * M_TO_CM, pose_rotation);
    }
}

void AViveSR_DualCameraImageRenderer::UpdateDistortedPose()
{
	auto pose_position = ViveSR_DualCameraImageCapture::Instance()->GetDistortedLocalPosition(DualCameraIndex::LEFT);
	auto pose_rotation = ViveSR_DualCameraImageCapture::Instance()->GetDistortedLocalRotation(DualCameraIndex::LEFT);

	FVector local_location(pose_position.X - 0.071, pose_position.Y + 0.0325, pose_position.Z + 0.0026);
	local_location = local_location * M_TO_CM;

	if (IsValid(vr_origin))
	{
		SetActorTransform(vr_origin->GetActorTransform());
		RootComponent->AddLocalTransform(FTransform(pose_rotation, local_location));
	}
	else
	{
		RootComponent->SetWorldLocationAndRotation(local_location, pose_rotation);
	}
}

UProceduralMeshComponent* AViveSR_DualCameraImageRenderer::GetDistortedImagePlane(DualCameraIndex eye)
{
	if (eye == DualCameraIndex::LEFT)
		return static_cast<AViveSR_DualCameraImagePlane*>(image_plane)->DistortedImagePlaneLeft;
	else
		return static_cast<AViveSR_DualCameraImagePlane*>(image_plane)->DistortedImagePlaneRight;
}
UStaticMeshComponent* AViveSR_DualCameraImageRenderer::GetUndistortedImagePlane(DualCameraIndex eye)
{
	if (eye == DualCameraIndex::LEFT)
		return static_cast<AViveSR_DualCameraImagePlane*>(image_plane)->UndistortedImagePlaneLeft;
	else
		return static_cast<AViveSR_DualCameraImagePlane*>(image_plane)->UndistortedImagePlaneRight;
}

UStaticMeshComponent* AViveSR_DualCameraImageRenderer::GetCalibratedImagePlane(DualCameraIndex eye)
{
    if (eye == DualCameraIndex::LEFT)
        return static_cast<AViveSR_DualCameraImagePlane*>(image_plane)->CalibratedImagePlaneLeft;
    else
        return static_cast<AViveSR_DualCameraImagePlane*>(image_plane)->CalibratedImagePlaneRight;
}

UStaticMeshComponent* AViveSR_DualCameraImageRenderer::GetDepthImagePlane()
{
	return static_cast<AViveSR_DualCameraImagePlane*>(image_plane)->DepthImagePlane;
}
