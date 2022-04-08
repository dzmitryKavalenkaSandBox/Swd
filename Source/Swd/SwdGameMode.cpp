// Copyright Epic Games, Inc. All Rights Reserved.

#include "SwdGameMode.h"
#include "SwdCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASwdGameMode::ASwdGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
