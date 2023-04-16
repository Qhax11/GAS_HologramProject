// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_TemplateGameMode.h"
#include "UObject/ConstructorHelpers.h"

AGAS_TemplateGameMode::AGAS_TemplateGameMode()
{
	// set default pawn class to our Blueprinted character
	/*
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/
}
