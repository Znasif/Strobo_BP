// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"

#include "Engine/World.h"
#include "Engine/Scene.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"

#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "GameFramework/Actor.h"
#include "HAL/PlatformFilemanager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "OptotypeComponent.generated.h"


UENUM(BlueprintType)
enum class MotionDirection : uint8 {
	Horizontal = 0 UMETA(DisplayName = "Horizontal"),
	Vertical = 1  UMETA(DisplayName = "Vertical"),
};

UENUM(BlueprintType)
enum class Gender : uint8 {
	Male = 0 UMETA(DisplayName = "Male"),
	Female = 1  UMETA(DisplayName = "Female"),
	Other = 2  UMETA(DisplayName = "Other"),
};


UENUM(BlueprintType)
enum class OptoOrientation : uint8 {
	Zero = 0 UMETA(DisplayName = "Left"), //
	Bottom_Left = 1 UMETA(DisplayName = "Bottom_Left"),
	Bottom = 2 UMETA(DisplayName = "Bottom"),
	Bottom_Right = 3 UMETA(DisplayName = "Bottom_Right"),
	Left = 4 UMETA(DisplayName = "Left"),
	Middle = 5 UMETA(DisplayName = "Right"),
	Right = 6 UMETA(DisplayName = "Right"),
	Top_Left = 7 UMETA(DisplayName = "Top_Left"),
	Top = 8 UMETA(DisplayName = "Top"),
	Top_Right = 9 UMETA(DisplayName = "Top_Right"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STROBO_BP_API UOptotypeComponent : public USceneComponent
{
	GENERATED_BODY()
	FTimerHandle OptotypeTimerHandle, StroboTimerHandle;
	FTimerDelegate OptotypeTimerDelegate, StroboTimerDelegate;
	float Elapsed_time = 0.0;
	float initial_logMAR = 1.4;
	//float distance = 0.0f;
	float temp_dist;;
	USceneComponent* optoPlane;
	UAudioComponent* metronome_comp;
	int total_opto_correct = 0;
	UCameraComponent* mycamera;
	bool strobo_state = true;
	float minification_array[12] = {1.0f, 0.7f, 0.5f, 0.3f, 0.2f, 1.0f, 0.7f, 0.5f, 0.3f, 0.2f, 1.0f, 1.0f};
	float response_mapping[10] = { 0.5, 0.625f, 0.75f, 0.875f, 0.5f, 0.0f, 0.0f, 0.375f, 0.25f, 0.125f };
	bool strobo_on_array[12] = { false, false, false, false, false, false, true, true, true, true, true, false };
	float result_per_session[12] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	FString genders[3] = { "Male", "Female", "Other" };
	int pos_in_session;
	UMaterialInstanceDynamic* post_camera_mat;
	UMaterialInstanceDynamic* stimuli_mat;
	FPostProcessSettings NormalSettings, PostSettings;
	float logMARtoGap(float logMAR);
public:	
	// Sets default values for this component's properties
	UOptotypeComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		float field_of_motion=30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		float metronome_frequency=2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		float optotype_frequency=2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		float stroboscopic_frequency=4.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		int start_session_id=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		int end_session_id=11;
	UPROPERTY(BlueprintReadWrite, Category = "Protocol Properties")
		bool session_in;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protocol Properties")
		MotionDirection motion_direction;
	UPROPERTY(BlueprintReadWrite, Category = "Protocol Properties")
		bool ready_to_quit_game = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		FString Subject_ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		Gender gender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		float Age;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		bool BCVA_2020;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		bool ocular_history_of_surgery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		bool neuro_balancing_disorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		bool seizure_vertigo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subject Information")
		float Laptop_Based_DVA_logMAR;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		int opto_orientation;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		float current_opto_size;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		FVector default_opto_position;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		FVector current_opto_position;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		float cur_offset;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		int total_opto_tested;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		float dir;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		float distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stimuli Properties")
		int How_Many_Opto=3;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		float previous_logMAR = 1.4;
	UPROPERTY(BlueprintReadWrite, Category = "Stimuli Properties")
		float current_logMAR = 1.3;
	UPROPERTY(BlueprintReadWrite, Category = "Countermeasure Properties")
		float minification = 1.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Countermeasure Properties")
		bool stroboscope_on;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Countermeasure Properties")
		float Aperture_Size=2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Countermeasure Properties")
		UMaterialInterface* parent_mat;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "DVA Function")
		void TestProtocol(ACameraActor* camera, USceneComponent* stimuli_plane, USceneComponent* background_plane, UAudioComponent* audio_comp, USoundCue* metronome_cue);
	UFUNCTION(BlueprintCallable, Category = "DVA Function")
		void NextSession();
	UFUNCTION(BlueprintCallable, Category = "DVA Function")
		void NextStimuli();
	UFUNCTION(BlueprintCallable, Category = "DVA Function")
		void AssessResponse(OptoOrientation response);
	UFUNCTION(BlueprintCallable, Category = "DVA Function")
		void SaveSessionData();
	UFUNCTION(BlueprintCallable, Category = "DVA Function")
		void SessionBreak();
	UFUNCTION(BlueprintCallable, Category = "DVA Function")
		void OneMotionCycle();
	UFUNCTION(BlueprintCallable, Category = "DVA Function")
		void OneStroboscopicFlicker();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = "CSV", meta = (Keywords = "Save"))
		static bool SaveArrayText(FString SaveDirectory, FString FileName, TArray<FString> SaveText, bool AllowOverwriting);
};
