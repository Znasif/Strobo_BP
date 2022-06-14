// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_SemanticObject.h"
#include "ViveSR_Enums.h"
#include "Misc/Paths.h"

ViveSR_SemanticObject* ViveSR_SemanticObject::Mgr = nullptr;
ViveSR_SemanticObject::ViveSR_SemanticObject()
{

}
ViveSR_SemanticObject::~ViveSR_SemanticObject()
{
}
ViveSR_SemanticObject* ViveSR_SemanticObject::Instance()
{
	if (Mgr == nullptr)
	{
		Mgr = new ViveSR_SemanticObject();
	}
	return Mgr;
}

int ViveSR_SemanticObject::GetElements(const FString &_xml_path, TArray<FElement> &Elements)
{
	xml_path = _xml_path;
	if (!FPaths::FileExists(_xml_path)) return ViveSR::Error::FAILED;

	FString tags;
	int32 ids=0;
	FString obj_file_name;
	FString cld_file_name;
	TArray<FVector> positions;
	FVector forwards;
	FVector box_min_point;
	FVector box_max_point;

	float x=0; float y = 0; float z = 0;
	FXmlFile* _xml_file = new FXmlFile(_xml_path);
	FXmlNode* _root_node = _xml_file->GetRootNode();
	const TArray<FXmlNode*> _asset_nodes = _root_node->GetChildrenNodes();
	for (FXmlNode* node : _asset_nodes)
	{
		FString tag_0 = node->GetTag();
		FString content_0 = node->GetContent();
		//UE_LOG(LogTemp, Warning, TEXT("Level0 %s %s"), *tag_0, *content_0);
		const TArray<FXmlNode*> _child_nodes = node->GetChildrenNodes();
		for (FXmlNode* node_1 : _child_nodes)
		{
			FString tag_1 = node_1->GetTag();
			FString content_1 = node_1->GetContent();
            if (tag_1 == "tag") tags = content_1;
			else if (tag_1 == "id") ids = FCString::Atoi(*content_1);
			else if (tag_1 == "obj_file_name") obj_file_name = content_1;
			else if (tag_1 == "cld_file_name") cld_file_name = content_1;
			else if (tag_1 == "position" || tag_1 == "forward"|| tag_1 == "bBoxMinPoint" || tag_1 == "bBoxMaxPoint") {
				
				const TArray<FXmlNode*> nodes_2 = node_1->GetChildrenNodes();
				for (FXmlNode* node_2 : nodes_2)
				{
					FString tag_2 = node_2->GetTag();
					FString content_2 = node_2->GetContent();
					if (tag_2 == "x") x = FCString::Atof(*content_2);
					else if (tag_2 == "y") y = FCString::Atof(*content_2);
					else if (tag_2 == "z") z = FCString::Atof(*content_2);
				}
				if (tag_1 == "position") positions.Add(FVector(x, y, z));
				else if (tag_1 == "forward") forwards = FVector(x, y, z);
				else if (tag_1 == "bBoxMinPoint") box_min_point = FVector(x, y, z);
				else if (tag_1 == "bBoxMaxPoint") box_max_point = FVector(x, y, z);	
			}
		}
	}	
	FElement element;
	element.tag = tags;
	element.id = ids;
	element.position = positions;
	element.forward = forwards;
	element.cld_file_name = cld_file_name;
	element.obj_file_name = obj_file_name;
	element.box_max_point = box_max_point;
	element.box_min_point = box_min_point;
	Elements.Add(element);	
	return 0;
}

 void ViveSR_SemanticObject::GetXmlPath(TArray<FString> &all_xml_path) {
	TArray<FString> file_names;
	FString app_path = FPlatformProcess::UserSettingsDir();
	app_path.RemoveFromEnd("Local/");
	FString folder_path = app_path + "LocalLow/HTC Corporation/SR_Reconstruction_Output/Recons3DAsset/SegmentDir/";
	const TCHAR *directory = *folder_path;
	FString filster = "xml";
	const TCHAR *extension = *filster;
	IFileManager::Get().FindFiles(file_names, directory, extension);
	all_xml_path.Init(folder_path, file_names.Num());
	for (int i = 0; i < file_names.Num(); i++) {
		all_xml_path[i].Append(file_names[i]);
	}
}
 void ViveSR_SemanticObject::CleanElement(TArray<FElement> &Elements) {
	 Elements.Empty();
 }
 
 void ViveSR_SemanticObject::GetBoxInfo(FElement Elements, FVector &box_center, FVector &box_extents,FColor &color) {
     FVector box_max_point = 100 * (FVector(Elements.box_max_point.Z, Elements.box_max_point.X, Elements.box_max_point.Y));
     FVector box_min_point = 100 * (FVector(Elements.box_min_point.Z, Elements.box_min_point.X, Elements.box_min_point.Y));
     box_center = 0.5*(box_max_point + box_min_point);
     box_extents = 0.5*(box_max_point - box_min_point);
    
     //Set color
 
     if (Elements.tag == "Chair") {
         color.R = 255; color.G = 0; color.B = 0; color.A = 0;
     }
     else if (Elements.tag == "Floor") {
         color.R = 0; color.G = 255; color.B = 0; color.A = 0;
     }
     else if (Elements.tag == "Wall") {
         color.R = 0; color.G = 0; color.B = 255; color.A = 0;
     }
     else if (Elements.tag == "Ceiling") {
         color.R = 192; color.G = 192; color.B = 0; color.A = 0;
     }
     else if (Elements.tag == "Table") {
         color.R = 0; color.G = 192; color.B = 192; color.A = 0;
     }
     else if (Elements.tag == "Bed") {
         color.R = 192; color.G = 0; color.B = 192; color.A = 0;
     }
     else if (Elements.tag == "Monitor") {
         color.R = 64; color.G = 0; color.B = 0; color.A = 0;
     }
     else if (Elements.tag == "Window") {
         color.R = 0; color.G = 64; color.B = 0; color.A = 0;
     }
     else if (Elements.tag == "Person") {
         color.R = 107; color.G = 142; color.B = 35; color.A = 0;
     }
     else if (Elements.tag == "Furniture") {
         color.R = 160; color.G = 32; color.B = 240; color.A = 0;
     }
     else if (Elements.tag == "Door") {
         color.R = 218; color.G = 112; color.B = 214; color.A = 0;
     }
     else if (Elements.tag == "Picture") {
         color.R = 227; color.G = 168; color.B = 105; color.A = 0;
     }
     else if (Elements.tag == "Light") {
         color.R = 255; color.G = 255; color.B = 120; color.A = 0;
     }
     else if (Elements.tag == "Plant") {
         color.R = 189; color.G = 252; color.B = 201; color.A = 0;
     }
     else if (Elements.tag == "Curtain") {
         color.R = 120; color.G = 60; color.B = 60; color.A = 0;
     }
     else if (Elements.tag == "Pillow") {
         color.R = 0; color.G = 0; color.B = 125; color.A = 0;
     }
     else {
         color.R = 32; color.G = 32; color.B = 32; color.A = 0;
     }

 }
 int ViveSR_SemanticObject::CheckChairNum() {
	 TArray<FString> _xml_path;
	 TArray<FElement>Elements;
	 int count = 0;
	 GetXmlPath(_xml_path);
	 for (int i = 0; i < _xml_path.Num(); i++) {
		GetElements(_xml_path[i], Elements);
	 }
	 for (int i = 0; i < Elements.Num(); i++) {
		 if (Elements[i].tag == L"Chair") {
			 count++;
		}
	 }
	 return count;
 }
  void ViveSR_SemanticObject::GetChairLocation(TArray<FVector>&Location) {
	 TArray<FString> _xml_path;
	 TArray<FElement>Elements;
	 FVector CoTransform;
	 int count = 0;
	 GetXmlPath(_xml_path);
	 for (int i = 0; i < _xml_path.Num(); i++) {
		 GetElements(_xml_path[i], Elements);
	 }
	 for (int i = 0; i < _xml_path.Num(); i++) {
		 if (Elements[i].tag == L"Chair") {
			 for (int j = 0; j < Elements[i].position.Num(); j++) {
				 CoTransform = FVector(Elements[i].position[j].Z, Elements[i].position[j].X, Elements[i].position[j].Y) * 100;
				 Location.Add(CoTransform);
			 }
		 }
		 count++;
	 }
 }

  void ViveSR_SemanticObject::GetChairForward(TArray<FVector>&Forward) {
	 TArray<FString> _xml_path;
	 TArray<FElement>Elements;
	 FVector CoTransform;
	 int count = 0;
	 GetXmlPath(_xml_path);
	 for (int i = 0; i < _xml_path.Num(); i++) {
		 GetElements(_xml_path[i], Elements);
	 }
	 for (int i = 0; i < Elements.Num(); i++) {
		 if (Elements[i].tag == L"Chair") {
			 for (int j = 0; j < Elements[i].position.Num(); j++) {
				 CoTransform = FVector(Elements[i].forward.Z, Elements[i].forward.X, Elements[i].forward.Y) * 100;
				 Forward.Add(CoTransform);
			 }
		 }
	 }
 }