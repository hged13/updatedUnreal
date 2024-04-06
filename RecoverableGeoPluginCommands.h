// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "RecoverableGeoPluginStyle.h"

class FRecoverableGeoPluginCommands : public TCommands<FRecoverableGeoPluginCommands>
{
public:

	FRecoverableGeoPluginCommands()
		: TCommands<FRecoverableGeoPluginCommands>(TEXT("RecoverableGeoPlugin"), NSLOCTEXT("Contexts", "RecoverableGeoPlugin", "RecoverableGeoPlugin Plugin"), NAME_None, FRecoverableGeoPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
