// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ViveSR_UEnums.h"
#include "ViveSR_RigidReconstructionRender.generated.h"

/**
* This is the actor can show the static mesh
* The actor can let user see the process of scanning in reconstruction
*/

UCLASS(Blueprintable)
class SRWORKS_API AViveSR_RigidReconstructionRender : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AViveSR_RigidReconstructionRender();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction|Render") void Initial();
    UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction|Render") void Release();
    UFUNCTION(BlueprintCallable, category = "ViveSR|RigidReconstruction|Render") void SetMeshMaterial(UMaterialInterface *material);
    UFUNCTION(BlueprintCallable, category = "ViveSR|SceneUnderstanding") bool GetisSceneUnderstanding();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "ViveSR|RigidReconstruction|Render") UMaterialInterface *MeshMaterial;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "ViveSR|RigidReconstruction|Render") UProceduralMeshComponent *MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "ViveSR|RigidReconstruction") bool EnablePreviewScanModel = false;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "ViveSR|RigidReconstruction|Render") GridSize MinGridSize = GridSize::FOUR;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "ViveSR|RigidReconstruction|Render") GridSize MaxGridSize = GridSize::SIXTYFOUR;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "ViveSR|RigidReconstruction|Render") float ErrorThres = 0.4F;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|RigidReconstruction") ReconstructionDisplayMode DisplayMode = ReconstructionDisplayMode::ADAPTIVE_MESH;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnabledSceneUnderstanding = false;


    // Choose SceneUnderstanding object in Editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableFloor = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableWall = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableCeiling = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableChair = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableTable = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableBed = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableMonitor = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableWindow = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableFurniture = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnablePicture = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableDoor = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnablePerson = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableLight = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnablePlant = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnableCurtain = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|SceneUnderstanding") bool EnablePillow = true;
private:
    float adaptive_range_min = 4.0F;
    float adaptive_range_max = 64.0F;
    int vertex_count;
    int index_count;
    int current_frame_id;
    int vertex_in_float;
    int last_processed_frame = 0;
    bool last_enabled_sceneunderstanding = false;

    TArray<float> vertex_data;
    TArray<int> index_data;
    TArray<FVector> normals;
    TArray<FVector2D> UV0;
    TArray<FProcMeshTangent> tangents;

    TArray<float> pose;
    TArray<FVector> valid_vertex_data;
    TArray<FColor> point_cloud_color;
    void Display();
    void SetSceneUnderstandingInfo();
    bool SetMeshDisplayMode(ReconstructionDisplayMode displayMode);

    TArray<int> index_preview;
    TArray<FVector> vertex_preview;
    TArray<FLinearColor> vertex_color_preview;
    uint8 *color_set;
};
