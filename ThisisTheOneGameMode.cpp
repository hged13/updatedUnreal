// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThisisTheOneGameMode.h"
#include "ThisisTheOneCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThisisTheOneGameMode::AThisisTheOneGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
