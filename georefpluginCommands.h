// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "georefpluginStyle.h"

class FgeorefpluginCommands : public TCommands<FgeorefpluginCommands>
{
public:

	FgeorefpluginCommands()
		: TCommands<FgeorefpluginCommands>(TEXT("georefplugin"), NSLOCTEXT("Contexts", "georefplugin", "georefplugin Plugin"), NAME_None, FgeorefpluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
