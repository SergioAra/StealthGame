// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

#include "EngineUtils.h"
#include "FPSCharacter.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bSuccess)
{
	for (TActorIterator<AFPSCharacter> It(GetWorld()); It; ++It)
	{
		if (AFPSCharacter* Character = *It)
		{
			Character->DisableInput(nullptr);
			
			if(AFPSPlayerController* Controller = Cast<AFPSPlayerController>(Character->GetController()))
			{
				if(Controller->IsLocalController())
				{
					Controller->OnMissionComplete(InstigatorPawn, bSuccess);
				}
				
			}
		}
	}
}
