// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "ProceduralMeshComponent.h"
#include "ViveSR.h"
#include "ViveSR_UEnums.h"
#include "ViveSR_DualCameraImagePlane.generated.h"

//#define USE_UNDISTORT_MESH

UCLASS()
class AViveSR_DualCameraImagePlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AViveSR_DualCameraImagePlane();
    /** Set owner of static mesh and prodedural mesh to the pawn in current level and let them no see to it
    * @warning If you want to switch level, Need to call this function to avoid vrcamera see mesh component in scene
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR")
    void SetMeshComponentNoSeeInVRCamera();

    /** Set owner meshComponet and WidgetComponent to the pawn in current level and let them only see to it
    * @warning If you want to switch to virtual mode, Need to call this function to avoid scene capture see mesh component in scene
    */
    UFUNCTION(BlueprintCallable, category = "ViveSR|Framework")
    void SetMeshComponentOnlySeeInVRCamera();

	int distorted_img_plane_width;
	int distorted_img_plane_height;
	int undistorted_img_plane_width;
	int undistorted_img_plane_height;
	int depth_img_plane_width;
    int depth_img_plane_height;
	float camera_focal_length;
	int undistortedCx[2];
	int undistortedCy[2];

	void Initial();
	void SetMode(DualCameraDisplayMode mode);
    void SetImgPlaneOwner(AActor* actor);

    UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImagePlane") UProceduralMeshComponent* DistortedImagePlaneLeft;
    UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImagePlane") UProceduralMeshComponent* DistortedImagePlaneRight;
    UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImagePlane") UStaticMeshComponent* UndistortedImagePlaneLeft;
    UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImagePlane") UStaticMeshComponent* UndistortedImagePlaneRight;
    UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImagePlane") UStaticMeshComponent* CalibratedImagePlaneLeft;
    UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImagePlane") UStaticMeshComponent* CalibratedImagePlaneRight;
    UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImagePlane") UStaticMeshComponent* DepthImagePlane;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    int M_TO_CM = 100;
    float SCENECAPTURE_FOV = 107.0f;
	FORCEINLINE void SetupImagePlaneMesh(UMeshComponent* Comp, FVector v= FVector(10000.0f, 0.0f, 0.0f))
	{
		if (!Comp) return;
		Comp->USceneComponent::AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
		Comp->SetRelativeLocation(v);
		Comp->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));
		Comp->SetCollisionProfileName("NoCollision");
		Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Comp->bCastDynamicShadow = false;
		Comp->CastShadow = false;
		Comp->SetHiddenInGame(true);
	}
    float CalculateFocalLength(float FoV, int img_width);
};
