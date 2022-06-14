#pragma once
#include "ViveSR_API_Enums.h"
#include <unordered_map>
enum ModuleTypeOfClient
{
    PASS_THROUGH = 0,
    DEPTH,
    DEPTHMESH,
    RIGID_RECONSTRUCTION,
    PASS_THROUGH_4K,
    CONTROLLER_POSE,
    AI_SCENE,
    MODULE_TYPE_MAX,
};
static const std::unordered_map<int, int> ModuleMapping({
    { ModuleTypeOfClient::PASS_THROUGH ,ViveSR::ModuleType::ENGINE_PASS_THROUGH },
    { ModuleTypeOfClient::DEPTH ,ViveSR::ModuleType::ENGINE_DEPTH },
    { ModuleTypeOfClient::RIGID_RECONSTRUCTION ,ViveSR::ModuleType::ENGINE_RIGID_RECONSTRUCTION },
    { ModuleTypeOfClient::PASS_THROUGH_4K , ViveSR::ModuleType::ENGINE_PASS_THROUGH_4K },
    { ModuleTypeOfClient::CONTROLLER_POSE ,ViveSR::ModuleType::ENGINE_CONTORLLER_POSE },
    { ModuleTypeOfClient::AI_SCENE, ViveSR::ModuleType::ENGINE_AI_SCENE }
});
