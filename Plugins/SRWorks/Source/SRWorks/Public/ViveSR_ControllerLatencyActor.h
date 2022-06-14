// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ViveSR_ControllerLatencyTool.h"
#include "ViveSR_UEnums.h"
#include "Math/Vector.h"
#include "ViveSR_ControllerLatencyActor.generated.h"


USTRUCT(BlueprintType)
struct FObjectToMoveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "ObjectToMoveData Struct") FVector Position;
    UPROPERTY(BlueprintReadWrite, Category = "ObjectToMoveData Struct") FRotator Rotation;
    UPROPERTY(BlueprintReadWrite, Category = "ObjectToMoveData Struct") FVector LocalPosition;

    FObjectToMoveData() : Position(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), LocalPosition(FVector::ZeroVector) {}
};

UCLASS(Blueprintable)
class SRWORKS_API AViveSR_ControllerLatencyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    AViveSR_ControllerLatencyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "ViveSR|ControllerLatency") ControllerHandType TrackController = ControllerHandType::NOHAND;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "ViveSR|ControllerLatency") int LatencyCompensation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "ViveSR|ControllerLatency") bool MoveAttachedObject = false;
    UFUNCTION(BlueprintCallable, category = "ViveSR|ControllerLatency") void AddObjectToMove(FVector position, FRotator rotation, FVector localPosition, FVector pos, FVector rot);
    UPROPERTY(BlueprintReadWrite, category = "ViveSR|ControllerLatency") TArray<FObjectToMoveData> ObjectToMoveDataInfo;
    
    void Initial();
    void Release();
    void UpdateControllerLocation();

private:
    std::chrono::time_point<std::chrono::steady_clock> start;
    TArray<ObjectToMoveData> object_to_move;
    FObjectToMoveData data_instance;
};
