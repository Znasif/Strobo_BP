//========= Copyright 2018, HTC Corporation. All rights reserved. ===========
#pragma once
namespace ViveSR {
    namespace SRWork {
        /** @enum ModuleStatus
        An enum type of the status of the specific engine.
        */
        enum ModuleStatus {
            MODULE_STATUS_ERROR,
            MODULE_STATUS_IDLE,
            MODULE_STATUS_WORKING,
            MODULE_STATUS_BLOCKED
        };
    }
}