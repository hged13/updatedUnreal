// Copyright Epic Games, Inc. All Rights Reserved.

#include "georefplugin.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Logging/LogMacros.h"
#include "FireDataStruct.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "FgeorefpluginModule"

void FgeorefpluginModule::StartupModule()
{




}


void FgeorefpluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FgeorefpluginModule, georefplugin)