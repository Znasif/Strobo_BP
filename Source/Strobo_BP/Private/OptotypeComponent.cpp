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

void UOptotypeComponent::TestProtocol(ACameraActor* camera, USceneComponent* stimuli_plane, USceneComponent* background_plane, UAudioComponent* audio_comp, USoundCue* metronome_cue)
{
	//FTimerHandle MetronomeTimerHandle, OptotypeTimerHandle, StroboTimerHandle;
	//	float metronome_frequency, optotype_frequency, stroboscopic_frequency;
	float stroboscopic_duration = 1.0f / stroboscopic_frequency;

	optoPlane = stimuli_plane;
	UStaticMeshComponent* opto = Cast<UStaticMeshComponent>(optoPlane);
	stimuli_mat = UMaterialInstanceDynamic::Create(opto->GetMaterial(0), this);
	opto->SetMaterial(0, stimuli_mat);
	mycamera = camera->GetCameraComponent();
	metronome_comp = audio_comp;
	//FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	//metronome_comp->AttachToComponent(optoPlane, rules);
	distance = FVector::Dist(background_plane->GetComponentLocation(), mycamera->GetComponentLocation())-10.0f;

	switch (motion_direction)
	{
	case MotionDirection::Horizontal:
		default_opto_position = FVector(0.0f, -distance * UKismetMathLibrary::DegTan(field_of_motion), 10.0f);
		break;
	case MotionDirection::Vertical:
		default_opto_position = FVector(distance * UKismetMathLibrary::DegTan(field_of_motion), 0.0f, 10.0f);
		break;
	}

	FSoundAttenuationSettings metronome_att_settings;
	metronome_att_settings.bAttenuate = true;
	metronome_att_settings.DistanceAlgorithm = EAttenuationDistanceModel::Linear;
	metronome_att_settings.AttenuationShape = EAttenuationShape::Sphere;
	metronome_att_settings.AttenuationShapeExtents.X = 1.0f;
	metronome_att_settings.FalloffDistance = 1000.0f;

	metronome_att_settings.bSpatialize = true;
	metronome_att_settings.SpatializationAlgorithm = ESoundSpatializationAlgorithm::SPATIALIZATION_HRTF;
	//metronome_att_settings.PluginSettings.SpatializationPluginSettingsArray
	metronome_att_settings.OmniRadius = 1.0f;
	metronome_att_settings.StereoSpread = 50.0f;

	metronome_comp->AdjustAttenuation(metronome_att_settings);
	metronome_comp->SetSound(metronome_cue);
	metronome_comp->SetIntParameter(FName("frequency"), (int)metronome_frequency);

	post_camera_mat = UMaterialInstanceDynamic::Create(parent_mat, this);
	post_camera_mat->SetScalarParameterValue(FName("aperture_radius"), Aperture_Size);
	TArray<FWeightedBlendable> post_blend;
	post_blend.Add(FWeightedBlendable(1.0, post_camera_mat));
	PostSettings.WeightedBlendables = post_blend;
	PostSettings.bOverride_AutoExposureBias = true;
	PostSettings.bOverride_BloomIntensity = true;
	PostSettings.bOverride_MotionBlurAmount = true;
	PostSettings.bOverride_AutoExposureMaxBrightness = true;
	PostSettings.bOverride_AutoExposureMinBrightness = true;
	PostSettings.AutoExposureBias = 0.0f;
	PostSettings.AutoExposureMaxBrightness = 1.0f;
	PostSettings.AutoExposureMinBrightness = 1.0f;
	PostSettings.BloomIntensity = 0.0f;
	PostSettings.MotionBlurAmount = 0.0f;
	NormalSettings = mycamera->PostProcessSettings;

	pos_in_session = start_session_id;
}

void UOptotypeComponent::NextSession() {
	if (ready_to_quit_game) {
		return;
	}
	session_in = true;
	previous_logMAR = initial_logMAR;
	current_logMAR = previous_logMAR - 0.1f;
	optoPlane->SetRelativeLocation(default_opto_position);
	
	current_opto_size = 2.0f *distance* logMARtoGap(initial_logMAR) /100.0f;
	current_opto_position = default_opto_position;
	optoPlane->SetRelativeScale3D(FVector(current_opto_size, current_opto_size, 0.0f));

	float metronome_duration = 1.0f / metronome_frequency, optotype_duration = 1.0f / optotype_frequency, stroboscopic_duration = 1.0f / stroboscopic_frequency;

	dir = 1.0f;
	OptotypeTimerDelegate.BindUFunction(this, FName("OneMotionCycle"));
	StroboTimerDelegate.BindUFunction(this, FName("OneStroboscopicFlicker"));

	metronome_comp->Play();

	if (pos_in_session <= end_session_id) {
		minification = minification_array[pos_in_session];
		stroboscope_on = strobo_on_array[pos_in_session];
		strobo_state = true;
		post_camera_mat->SetScalarParameterValue(FName("magnify"), 1.0 / minification);
		post_camera_mat->SetScalarParameterValue(FName("strobe"), 1.0);
		mycamera->PostProcessSettings = PostSettings;
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("PostProcessing : %f %d"), minification, stroboscope_on));
	}
	else {
		ready_to_quit_game = true;
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
		return;
	}
	if (optotype_duration > 0.0f) GetWorld()->GetTimerManager().SetTimer(OptotypeTimerHandle, OptotypeTimerDelegate, 0.005f, true, 0.0f);
	if (stroboscopic_duration > 0.0f) GetWorld()->GetTimerManager().SetTimer(StroboTimerHandle, StroboTimerDelegate, stroboscopic_duration, true, 0.0f);
	
	opto_orientation = 6;// OptoOrientation::Right;
	total_opto_tested = 0;
	total_opto_correct = 0;
	NextStimuli();
}

void UOptotypeComponent::SessionBreak() {
	metronome_comp->Stop();
	
	GetWorld()->GetTimerManager().ClearTimer(OptotypeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StroboTimerHandle);
	mycamera->PostProcessSettings = NormalSettings;
	session_in = false;
}

void UOptotypeComponent::SaveSessionData()
{
	TArray < FString> DVA_Data = { "Subject ID, Best Correctable Vision, Age, Gender, Any ocular history or surgery, Any neurological/balancing disorder, Any seizures or vertigo?, Laptop based DVA, DVA with Headset,	Minifying lens (30%) smaller DVA, Minifying lens (50%) smaller DVA,	Minifying lens (70%) smaller DVA, Minifying lens (80%) smaller DVA,	DVA with Headset (no countermeasures), Minifying lens (30%) smaller + Stroboscopic DVA,	Minifying lens (50%) smaller +  Stroboscopic DVA, Minifying lens (70%) smaller +  Stroboscopic DVA,	Minifying lens (80%) smaller +  Stroboscopic DVA,	Stroboscopic Effect only- Measure DVA,	Measure DVA with headset (again)" };
	FString temp;
	temp += Subject_ID + ",";
	if (BCVA_2020) temp += "Yes,";
	else temp += "No,";
	temp += FString::SanitizeFloat(Age)+",";
	temp += genders[(int)gender]+",";
	if(ocular_history_of_surgery) temp += "Yes,";
	else temp += "No,";
	if(neuro_balancing_disorder) temp += "Yes,";
	else temp += "No,";
	if(seizure_vertigo) temp += "Yes,";
	else temp += "No,";
	//Laptop_based_DVA
	temp += FString::SanitizeFloat(Laptop_Based_DVA_logMAR)+",";
	//DVA with Headset
	temp += FString::SanitizeFloat(result_per_session[0])+",";
	//Minifying lens (30%) smaller DVA
	temp += FString::SanitizeFloat(result_per_session[1]) + ",";
	//Minifying lens (50%) smaller DVA
	temp += FString::SanitizeFloat(result_per_session[2]) + ",";
	//Minifying lens (70%) smaller DVA
	temp += FString::SanitizeFloat(result_per_session[3]) + ",";
	//Minifying lens (80%) smaller DVA
	temp += FString::SanitizeFloat(result_per_session[4]) + ",";
	//DVA with Headset (no countermeasures)
	temp += FString::SanitizeFloat(result_per_session[5]) + ",";
	//Minifying lens (30%) smaller + Stroboscopic DVA
	temp += FString::SanitizeFloat(result_per_session[6]) + ",";
	//Minifying lens (50%) smaller + Stroboscopic DVA
	temp += FString::SanitizeFloat(result_per_session[7]) + ",";
	//Minifying lens (70%) smaller + Stroboscopic DVA
	temp += FString::SanitizeFloat(result_per_session[8]) + ",";
	//Minifying lens (80%) smaller + Stroboscopic DVA
	temp += FString::SanitizeFloat(result_per_session[9]) + ",";
	//Stroboscopic Effect only- Measure DVA
	temp += FString::SanitizeFloat(result_per_session[10]) + ",";
	//Measure DVA with headset (again)
	temp += FString::SanitizeFloat(result_per_session[11]) + ",";

	DVA_Data.Add(temp);

	FString SaveLocation = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir());
	SaveArrayText(SaveLocation, Subject_ID+"_"+FString::FromInt(start_session_id)+"_"+ FString::FromInt(end_session_id) +"_DVA.csv", DVA_Data, true);
	pos_in_session++;
	session_in = false;
	SessionBreak();
}

void UOptotypeComponent::NextStimuli()
{
	opto_orientation = FMath::Fmod(opto_orientation + FMath::RandRange(1, 9), 10);
	if(opto_orientation == 5){
		opto_orientation = 6;
	}
	if (opto_orientation == 0) {
		opto_orientation = 4;
	}
	stimuli_mat->SetScalarParameterValue(FName("orientation"), response_mapping[opto_orientation]);
}

void UOptotypeComponent::AssessResponse(OptoOrientation response) {
	total_opto_tested++;
	if ((int)response == opto_orientation) 	total_opto_correct++;
	if (total_opto_tested < How_Many_Opto) {
		NextStimuli();
	}
	else {
		if (2 * total_opto_correct > total_opto_tested) {
			previous_logMAR = current_logMAR;
			current_logMAR -= 0.1f;
		}
		else {
			current_logMAR = (current_logMAR + previous_logMAR) / 2.0f;
		}
		if (FMath::Abs(current_logMAR - previous_logMAR) < 0.03f) {
			result_per_session[pos_in_session] = current_logMAR;
			/*if (pos_in_session == 0) {
				initial_logMAR = previous_logMAR;
			}*/
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("LogMAR Difference: %d"), FMath::Abs(current_logMAR - previous_logMAR)));
			SaveSessionData();
		}
		current_opto_size = 2.0f * distance * logMARtoGap(current_logMAR) / 100.0f;
		current_opto_position = default_opto_position;
		optoPlane->SetRelativeScale3D(FVector(current_opto_size, current_opto_size, 0.0f));
		total_opto_tested = 0;
		total_opto_correct = 0;
	}
}

void UOptotypeComponent::OneMotionCycle() {
	Elapsed_time += 0.005f;
	cur_offset = FVector::Dist(current_opto_position, default_opto_position);
	temp_dist = 2 * distance * UKismetMathLibrary::DegTan(field_of_motion);
	if (cur_offset >= temp_dist) {
		dir = -1.0f;
	}
	else if (cur_offset <= 0) {
		dir = 1.0f;;
	}
	float travel_to = dir *FMath::Abs( 0.005 * temp_dist * optotype_frequency);
	switch (motion_direction)
	{
	case MotionDirection::Horizontal:
		current_opto_position = FVector(current_opto_position.X, current_opto_position.Y + travel_to, current_opto_position.Z);
		break;
	case MotionDirection::Vertical:
		current_opto_position = FVector(current_opto_position.X + travel_to, current_opto_position.Y, current_opto_position.Z);
		break;
	}
	optoPlane->SetRelativeLocation(current_opto_position);
}

void UOptotypeComponent::OneStroboscopicFlicker() {
	if (stroboscope_on) {
		if (strobo_state) {
			post_camera_mat->SetScalarParameterValue(FName("strobe"), 1.0);
		}
		else {
			post_camera_mat->SetScalarParameterValue(FName("strobe"), 0.0);
		}
		strobo_state = !strobo_state;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 0.02f, FColor::Red, FString::Printf(TEXT("PostProcessing : %d"), strobo_state));
}

float UOptotypeComponent::logMARtoGap(float logMAR)
{
	return UKismetMathLibrary::DegTan(FMath::Pow(10, logMAR) / 12.0f);
}
