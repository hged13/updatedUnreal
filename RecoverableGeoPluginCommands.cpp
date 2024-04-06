// Copyright Epic Games, Inc. All Rights Reserved.

#include "RecoverableGeoPluginCommands.h"

#define LOCTEXT_NAMESPACE "FRecoverableGeoPluginModule"

void FRecoverableGeoPluginCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "RecoverableGeoPlugin", "Execute RecoverableGeoPlugin action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
