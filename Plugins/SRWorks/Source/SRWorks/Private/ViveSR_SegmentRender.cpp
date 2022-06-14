// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_SegmentRender.h"
#include "ViveSR_Enums.h"
#include "SRWorks.h"

// Sets default values
AViveSR_SegmentRender::AViveSR_SegmentRender()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AViveSR_SegmentRender::BeginPlay()
{
	Super::BeginPlay();
	// Get information from xml
    ViveSR_SemanticObject::Instance()->GetXmlPath(xml_path);
    for (int i = 0; i < xml_path.Num(); i++){
        ViveSR_SemanticObject::Instance()->GetElements(xml_path[i], Elements);
    }
}

void AViveSR_SegmentRender::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ViveSR_SemanticObject::Instance()->CleanElement(Elements);
}
void AViveSR_SegmentRender::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    if (EnableGetObject) {
        ViveSR_SemanticObject::Instance()->GetXmlPath(xml_path);
        for (int i = 0; i < xml_path.Num(); i++) {
            ViveSR_SemanticObject::Instance()->GetElements(xml_path[i], Elements);
        }
        EnableGetObject = false;
    }
    if (EnableBox) {
        for (int i = 0; i < Elements.Num(); i++) {
            ViveSR_SemanticObject::Instance()->GetBoxInfo(Elements[i], box_center, box_extents, color);
            DrawDebugBox(GetWorld(), box_center, box_extents, color, false, 0, 0, 0.5);
        }
    }
}