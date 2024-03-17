// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"

#include "EngineUtils.h"
#include "FPSCharacter.h"
#include "FPSHUD.h"
#include "FPSGameState.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
	
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bSuccess)
{
	if(InstigatorPawn)
	{
		if(!SpectatorViewpointClass) return;
		
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatorViewpointClass, OutActors);

		//Change view target if any valid actor is found
		if(OutActors.Num() > 0)
		{
			AActor* NewViewTarget = OutActors[0];

			//Iterate over characters
			for (TActorIterator<AFPSCharacter> It(GetWorld()); It; ++It)
			{
				if (AFPSCharacter* Character = *It)
				{
					if(APlayerController* Controller = Cast<APlayerController>(Character->GetController()))
					{
						Controller->SetViewTargetWithBlend(NewViewTarget, 0.5f, VTBlend_Cubic);
				
					}	
				}
			}
		}
	}

	//Call GameState multicast to disable input on local players
	if(AFPSGameState* CurrentGameState = GetGameState<AFPSGameState>())
	{
		CurrentGameState->MulticastOnMissionComplete(InstigatorPawn, bSuccess);
	}
	
	OnMissionCompleted(InstigatorPawn, bSuccess);

	
}

