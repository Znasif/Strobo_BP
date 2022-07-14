// Fill out your copyright notice in the Description page of Project Settings.


#include "OptotypeComponent.h"

// Sets default values for this component's properties
UOptotypeComponent::UOptotypeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOptotypeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOptotypeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UOptotypeComponent::SaveArrayText(FString SaveDirectory, FString FileName, TArray<FString> SaveText, bool AllowOverwriting = false)
{
	// Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverwriting)
	{
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			return false;
		}
	}

	FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*SaveDirectory);
	FString FinalString = "";
	for (FString& Each : SaveText)
	{
		FinalString += Each;
		FinalString += LINE_TERMINATOR;
	}

	return FFileHelper::SaveStringToFile(FinalString, *SaveDirectory);

}

void UOptotypeComponent::TestProtocol(ACameraActor* camera, USceneComponent* stimuli_plane, USoundWave* metronome_wave)
{
	//FTimerHandle MetronomeTimerHandle, OptotypeTimerHandle, StroboTimerHandle;
	FTimerDelegate MetronomeTimerDelegate, OptotypeTimerDelegate, StroboTimerDelegate;
	//	float metronome_frequency, optotype_frequency, stroboscopic_frequency;
	float metronome_duration = 1.0f / metronome_frequency, optotype_duration = 1.0f / optotype_frequency, stroboscopic_duration = 1.0f / stroboscopic_frequency;

	optoPlane = stimuli_plane;
	mycamera = camera->GetCameraComponent();
	metronome_comp = UAudioComponent()

	post_camera_mat = UMaterialInstanceDynamic::Create(parent_mat, this);
	TArray<FWeightedBlendable> post_blend;
	post_blend.Add(FWeightedBlendable(1.0, post_camera_mat));
	PostSettings.WeightedBlendables = post_blend;
	NormalSettings = mycamera->PostProcessSettings;

	MetronomeTimerDelegate.BindUFunction(this, FName("OneMetronomeCycle"));
	OptotypeTimerDelegate.BindUFunction(this, FName("OneMotionCycle"));
	StroboTimerDelegate.BindUFunction(this, FName("OneStroboscopicFlicker"));

	if (metronome_duration > 0.0f) GetWorld()->GetTimerManager().SetTimer(MetronomeTimerHandle, MetronomeTimerDelegate, metronome_duration, true, 0.0f);
	if (optotype_duration > 0.0f) GetWorld()->GetTimerManager().SetTimer(OptotypeTimerHandle, OptotypeTimerDelegate, 0.005f, true, 0.0f);
	if (stroboscopic_duration > 0.0f) GetWorld()->GetTimerManager().SetTimer(StroboTimerHandle, StroboTimerDelegate, stroboscopic_duration, true, 0.0f);
}

void UOptotypeComponent::NextSession() {
	distance = UKismetMathLibrary::Vector_Distance(mycamera->GetComponentLocation(), optoPlane->GetComponentLocation())/10.0f;
	current_opto_size = distance*UKismetMathLibrary::DegTan(20.0f/12.0f);
	optoPlane->SetRelativeScale3D(FVector(current_opto_size, current_opto_size, 0.0f));
}

void OneMetronomeCycle() {
	
}

void OneMotionCycle() {

}

void OneStroboscopicFlicker() {

}