// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ViveSR_DualCameraDepthCollider.generated.h"

/**
* This is the actor can show the dynamic mesh
* The actor can let user see the depth mesh
*/

UCLASS(Blueprintable)
class SRWORKS_API AViveSR_DualCameraDepthCollider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AViveSR_DualCameraDepthCollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Release();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "ViveSR|DualCamera|DepthCollider") bool DepthColliderVisibility = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "ViveSR|DualCamera|DepthCollider") bool UpdateDepthCollider = false;

	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|DepthCollider") int EnableDepthCollider(bool enable);
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|DepthCollider") void SetLiveMeshVisibility(bool visible);
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|DepthCollider") bool GetQualityScale(int &value);
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|DepthCollider") bool SetQualityScale(int value);
	UPROPERTY(BlueprintReadOnly, category = "ViveSR|DualCamera|DepthCollider") UProceduralMeshComponent *MeshComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "ViveSR|DualCamera|DepthCollider") UMaterialInterface *MeshMaterial;
private:
	int quality_scale = 8;
	int last_update_time;	
	int vertex_data_num;

    bool LastUpdateDepthCollider = false;

	TArray<float> vertex_data;
	int idx_data_num;
	TArray<int> idx_data;	

	void ExtractCurrentColliders();
	void GenerateMeshColliders();
};
