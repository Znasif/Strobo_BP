// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "ViveSR_SegmentRender.h"
#include "ViveSR_DualCameraImageCapture.h"
#include "ViveSR_DualCameraDepthExtra.h"
#include "ViveSR_DualCameraImagePlane.h"
#include "SRWorks.h"
#include "ViveSR_RigidReconstruction.h"
// Sets default values
AViveSR::AViveSR()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void AViveSR::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AViveSR::StaticClass(), FoundActors);
	if (FoundActors.Num() > 1)
	{
		UE_LOG(LogViveSR, Log, TEXT("[ViveSR] Please make only one ViveSR in your content"));
	}
	StartFramework();
}

void AViveSR::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopFramework();
}

void AViveSR::StartFramework()
{
	if (Status == FrameworkStatus::WORKING) return;	
	int result = FSRWorksModule::Get().StartFramework();
	if (result != ViveSR::Error::WORK)
	{
		UE_LOG(LogViveSR, Warning, TEXT("Start Framework: %d"), result);
        Status = FrameworkStatus::ERROR_;
        if (ViveSR::Error::EULA_NOT_ACCEPT == result) {
            FGenericPlatformMisc::RequestExit(true);
        }
		return;
	}
    result = FSRWorksModule::Get().UnLinkModule();
    if (result != ViveSR::Error::WORK)
    {
        UE_LOG(LogViveSR, Warning, TEXT("Unlink Module: %d"), result);
        Status = FrameworkStatus::ERROR_;
        return;
    }
	Status = FrameworkStatus::WORKING;

	// Intial DualCameraImageRenderer
	if (!IsValid(DualCameraImageRenderer))	// Get the reference if it exists.
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AViveSR_DualCameraImageRenderer::StaticClass(), foundActors);
		if (foundActors.Num() > 0)
			DualCameraImageRenderer = (AViveSR_DualCameraImageRenderer*)foundActors[0];
	}
	if (IsValid(DualCameraImageRenderer)) {
		DualCameraImageRenderer->Initial();
		if (IsValid(VROrigin))
		{
			DualCameraImageRenderer->SetVROrigin(VROrigin);
		}
		ViveSR_DualCameraDepthExtra::Instance()->InitialDepthCollider(ViveSR_DualCameraImageCapture::Instance()->depth_img_width, ViveSR_DualCameraImageCapture::Instance()->depth_img_height);
	}
	
	// Intial DualCameraDepthCollider
	if (!IsValid(DualCameraDepthCollider)) // Get the reference if it exists.
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AViveSR_DualCameraDepthCollider::StaticClass(), foundActors);
		if (foundActors.Num() > 0)
			DualCameraDepthCollider = (AViveSR_DualCameraDepthCollider*)foundActors[0];
	}
	
	// Intial RigidReconstructionRender
	if (!IsValid(RigidReconstructionRender)) // Get the reference if it exists.
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AViveSR_RigidReconstructionRender::StaticClass(), foundActors);
		if (foundActors.Num() > 0)
			RigidReconstructionRender = (AViveSR_RigidReconstructionRender*)foundActors[0];
	}
	if (IsValid(RigidReconstructionRender))
	{
		RigidReconstructionRender->Initial();
	}
    // Intial ViveSR_ControllerLatencyActor
    if (ControllerLatencyActor.Num()==0) // Get the reference if it exists.
    {
        TArray<AActor*> foundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AViveSR_ControllerLatencyActor::StaticClass(), foundActors);
        if (foundActors.Num() > 0) {
            for (int i = 0; i < foundActors.Num(); i++) {
                ControllerLatencyActor.Add((AViveSR_ControllerLatencyActor*)foundActors[i]);
            }         
        }
    }
}

void AViveSR::StopFramework()
{
	if (Status != FrameworkStatus::STOP)
	{
		Status = FrameworkStatus::STOP;
		if (IsValid(DualCameraImageRenderer)) DualCameraImageRenderer->Release();
		if (IsValid(DualCameraDepthCollider)) DualCameraDepthCollider->Release();
		if (IsValid(RigidReconstructionRender)) RigidReconstructionRender->Release();
		int result = FSRWorksModule::Get().StopFramework();
		UE_LOG(LogViveSR, Warning, TEXT("Stop Framework: %d"), result);
	}
	else
	{
		UE_LOG(LogViveSR, Warning, TEXT("Stop Framework: not working"));
	}
}

void AViveSR::DestroyLevelActor() {
	TArray<TSubclassOf<AActor>> srworks_class{ AViveSR_DualCameraImageRenderer::StaticClass(),
        AViveSR_DualCameraImagePlane::StaticClass(), 
        AViveSR_DualCameraDepthCollider::StaticClass(),
        AViveSR_RigidReconstructionRender::StaticClass(),
        AViveSR_SegmentRender::StaticClass(),
        AViveSR_ControllerLatencyActor::StaticClass() };
	TArray<AActor*> foundActors;
	for (auto const& sr_class : srworks_class) {
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), sr_class, foundActors);
		for (auto const& actor : foundActors) {
			if (IsValid(actor))
				GetWorld()->DestroyActor(actor, false, true);
		}
	}
    UE_LOG(LogViveSR, Warning, TEXT("DestroyLevelActor: finish"));
}

