// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSGuard.generated.h"

class UPawnSensingComponent;


UENUM(BlueprintType)
enum class EGState : uint8
{
	Idle,

	Suspicious,

	Alerted
};

UCLASS()
class FPSGAME_API AFPSGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSGuard();

private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	FRotator StartRotation;

	FTimerHandle DistractionTimer;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DistractionTime;

	UPROPERTY(ReplicatedUsing = OnRep_GuardState)
	EGState GuardState;

	//only called when the variable changes, runs on the client
	UFUNCTION()
	void OnRep_GuardState();

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
	TArray<AActor*> PatrolPoints;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	int32 CurrentPatrolPointIndex;

	float MinDistanceToPatrolPoint;

	FTimerHandle PatrolTimer;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolTime;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void ResetOrientation();

	void SetGuardState(EGState NewState);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnGuardStateChanged(EGState NewState);

	void MoveToNextPatrolPoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
