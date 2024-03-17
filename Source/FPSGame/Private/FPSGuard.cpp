// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGuard.h"

#include "FPSGameMode.h"
#include "UnrealNetwork.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AFPSGuard::AFPSGuard():
	DistractionTime(3.f),
	GuardState(EGState::Idle),
	bPatrol(false),
	CurrentPatrolPointIndex(-1.f),
	MinDistanceToPatrolPoint(50.f),
	PatrolTime(3.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing comp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSGuard::OnNoiseHeard);

}

void AFPSGuard::OnRep_GuardState()
{
	OnGuardStateChanged(GuardState);
}

// Called when the game starts or when spawned
void AFPSGuard::BeginPlay()
{
	Super::BeginPlay();

	StartRotation = GetActorRotation();

	if(bPatrol)
	{
		MoveToNextPatrolPoint();
	}
	
}

void AFPSGuard::OnPawnSeen(APawn* SeenPawn)
{
	if(!SeenPawn) return;

	//Get Game Mode from server
	AFPSGameMode* GameMode =  Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		GameMode->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EGState::Alerted);

	
	if(GetController())
	{
		GetController()->StopMovement();
	}
	
}

void AFPSGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if(GuardState == EGState::Alerted) return;
		
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(DistractionTimer);
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	GetWorldTimerManager().SetTimer(DistractionTimer, this, &AFPSGuard::ResetOrientation, DistractionTime, false);


	SetGuardState(EGState::Suspicious);

	if(GetController())
	{
		GetController()->StopMovement();
	}
	
	
}

void AFPSGuard::ResetOrientation()
{
	if(GuardState == EGState::Alerted) return;
	SetActorRotation(StartRotation);
	SetGuardState(EGState::Idle);

	if(bPatrol)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), PatrolPoints[CurrentPatrolPointIndex]);
	}
}

void AFPSGuard::SetGuardState(EGState NewState)
{
	if(GuardState == NewState) return;
	
	GuardState = NewState;

	OnRep_GuardState();
}

void AFPSGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSGuard, GuardState);
}


void AFPSGuard::MoveToNextPatrolPoint()
{
	if(CurrentPatrolPointIndex < PatrolPoints.Num()-1)
	{
		CurrentPatrolPointIndex++;
	}
	else
	{
		CurrentPatrolPointIndex = 0;
	}
	
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), PatrolPoints[CurrentPatrolPointIndex]);
}

// Called every frame
void AFPSGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bPatrol && CurrentPatrolPointIndex >= 0 && PatrolPoints[CurrentPatrolPointIndex])
	{
		FVector Delta = GetActorLocation() - PatrolPoints[CurrentPatrolPointIndex]->GetActorLocation();
		if(Delta.Size() < MinDistanceToPatrolPoint)
		{
			if(!GetWorldTimerManager().IsTimerActive(PatrolTimer))
			{
				GetWorldTimerManager().SetTimer(PatrolTimer, this, &AFPSGuard::MoveToNextPatrolPoint, PatrolTime, false);
			}
		}
	}

}



