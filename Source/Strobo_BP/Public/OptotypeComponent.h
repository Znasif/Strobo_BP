// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Engine/Scene.h"
#include "Misc/FileHelper.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "HAL/PlatformFilemanager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "OptotypeComponent.generated.h"


UENUM(BlueprintType)
enum class OptoDirection : uint8 {
	Horizontal = 0 UMETA(DisplayName = "Horizontal"),
	Vertical = 1  UMETA(DisplayName = "Vertical"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STROBO_BP_API UOptotypeComponent : public UActorComponent
{
	GENERATED_BODY()
	FTimerHandle MetronomeTimerHandle, OptotypeTimerHandle, StroboTimerHandle;
	float Elapsed_time = 0.0;
	float initial_opto_size = 1.3;
	float last_opto_size = 1.4;
	bool response = {};
	float distance = 0.0f;
	USceneComponent* optoPlane;
	UAudioComponent* metronome_comp;
	UCameraComponent* mycamera;
	float minification_array[12] = {1.0f, 0.3f, 0.5f, 0.7f, 0.8f, 1.0f, 0.3f, 0.5f, 0.7f, 0.8f, 1.0f, 1.0f};
	bool strobo_on_array[12] = { false, false, false, false, false, false, true, true, true, true, true, false };
	UMaterialInstanceDynamic* post_camera_mat;
	FPostProcessSettings NormalSettings, PostSettings;
	void OneMetronomeCycle();
	void OneMotionCycle();
	void OneStroboscopicFlicker();
public:	
	// Sets default values for this component's properties
	UOptotypeComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		float field_of_motion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		float metronome_frequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		float optotype_frequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		float stroboscopic_frequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		OptoDirection motion_direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		FString Subject_ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		bool BCVA_2020;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		bool ocular_history_of_surgery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		bool neuro_balancing_disorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		bool seizure_vertigo;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		float opto_orientation;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		float current_opto_size;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		float current_opto_position;
	UPROPERTY(BlueprintReadWrite, Category = "Countermeasure Properties")
		float minification = 1.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Countermeasure Properties")
		bool stroboscope_on;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Countermeasure Properties")
		UMaterialInterface* parent_mat;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Function")
		void TestProtocol(ACameraActor* camera, USceneComponent* stimuli_plane, USoundWave* metronome_wave);
	UFUNCTION(BlueprintCallable, Category = "Function")
		void NextSession();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = "CSV", meta = (Keywords = "Save"))
		static bool SaveArrayText(FString SaveDirectory, FString FileName, TArray<FString> SaveText, bool AllowOverwriting);
};
