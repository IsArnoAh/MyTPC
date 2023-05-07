// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTPCGameMode.h"
#include "MyTPCCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyTPCGameMode::AMyTPCGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
