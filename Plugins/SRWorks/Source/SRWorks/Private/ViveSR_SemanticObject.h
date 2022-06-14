// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ViveSR_UEnums.h"
#include "HAL/FileManager.h"
#include "Runtime/XmlParser/Public/XmlParser.h"

struct FElement
{
	FString tag;
	int32 id;
	FVector forward;
	FString obj_file_name;
	FString cld_file_name;
	TArray<FVector> position;
	FVector box_min_point;
	FVector box_max_point;
};


/**
 *
 */
class ViveSR_SemanticObject
{
public:
	ViveSR_SemanticObject();
	~ViveSR_SemanticObject();


	static ViveSR_SemanticObject* Instance();
	int GetElements(const FString &_xml_path, TArray<FElement> &Elements);
	FString xml_path;
	void GetXmlPath(TArray <FString>& all_xml_path);
	void CleanElement(TArray<FElement> &Elements);
    void GetBoxInfo(FElement Elements, FVector &box_center, FVector &box_extents, FColor &color);

	int CheckChairNum();
	void GetChairLocation(TArray<FVector>&Location);
	void GetChairForward(TArray<FVector>&Forward);
private:
	static ViveSR_SemanticObject *Mgr;
};
