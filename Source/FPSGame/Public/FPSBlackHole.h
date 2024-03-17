// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBlackHole.generated.h"

class USphereComponent;

UCLASS()
class FPSGAME_API AFPSBlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSBlackHole();

private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	//Sphere used to attract overlapping objects
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* AttractSphere;

	//Sphere used to destroy overlapping objects
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* DestroySphere;

	UPROPERTY(VisibleAnywhere)
	float SphereRadius;

	UPROPERTY(EditAnywhere)
	float ForceStrength;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDestroySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
