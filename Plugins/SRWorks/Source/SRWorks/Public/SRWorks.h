// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FSRWorksModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline FSRWorksModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FSRWorksModule >("SRWorks");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("SRWorks");
	}
	static inline FSRWorksModule& GetModuleChecked()
	{
		return FModuleManager::GetModuleChecked<FSRWorksModule>("SRWorks");
	}

	int StartFramework();
	int StopFramework();
	int StartModule(int moduletype);
	int StopModule(int moduletype);
    int UnLinkModule();

private:
	bool LoadLibraryDLL(const FString& libName, void*& handle);

	//static FSRWorksModule* SRWorkModule;
	/** Handle to the SRWorks dll */
	void* SRWorksAPIDllHandle; 

	FString BaseDir;
	int SRWorksResult;

public:
};