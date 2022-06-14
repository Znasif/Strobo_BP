// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ViveSR_DualCameraImageRenderer.h"
#include "ViveSR_DualCameraDepthCollider.h"
#include "ViveSR_RigidReconstructionRender.h"
#include "ViveSR_ControllerLatencyActor.h"
#include "SRWork_Client_API.h"
#include "ViveSR_UEnums.h"
#include "ViveSR.generated.h"
DEFINE_LOG_CATEGORY_STATIC(LogViveSR, Log, All)

/**
* This is the actor Enable to start framework and stop framework
* The actor can let user initial and release resource 
*/

UCLASS()
class SRWORKS_API AViveSR : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AViveSR();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR") AActor* VROrigin = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Initial ViveSR all modules and actors.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR")
	void StartFramework();
	/** Release ViveSR all modules and actors.
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR")
	void StopFramework();
	/** Release all resource of actors in current level
	* @warning If you want to unload a level during playing, this function is required
    */
	UFUNCTION(BlueprintCallable, category = "ViveSR")
	void DestroyLevelActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViveSR") FrameworkStatus Status = FrameworkStatus::STOP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR") AViveSR_DualCameraImageRenderer *DualCameraImageRenderer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR") AViveSR_DualCameraDepthCollider *DualCameraDepthCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR") AViveSR_RigidReconstructionRender *RigidReconstructionRender;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR") TArray<AViveSR_ControllerLatencyActor*> ControllerLatencyActor;
};
