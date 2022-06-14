// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ViveSR_UEnums.h"
#include "ViveSR_SemanticObject.h"
#include "DrawDebugHelpers.h"
#include "ViveSR_SegmentRender.generated.h"

/**
* This is the actor can show the bounding box of each semantic object.
* The actor can let user see the location and size of each semantic object.
*/

UCLASS(Blueprintable)
class SRWORKS_API AViveSR_SegmentRender : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AViveSR_SegmentRender();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
    virtual void Tick(float DeltaTime) override;
    /** Enable boundingbox or not. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|BoundingBox") bool EnableBox = false;
    /** Enable get semantic object data or not. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|BoundingBox") bool EnableGetObject = false;

private:
	TArray<FString> xml_path;
	TArray<FElement>Elements;

protected:
    FVector box_center;
    FVector box_extents;
    FColor color;
};
