// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"

#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole():
	ForceStrength(-2000.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;
	
	AttractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttractSphere"));
	AttractSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttractSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	AttractSphere->SetSphereRadius(3000.f);
	AttractSphere->SetupAttachment(RootComponent);

	DestroySphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestroySphere"));
	DestroySphere->SetSphereRadius(100.f);
	DestroySphere->SetupAttachment(RootComponent);
	DestroySphere->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OnDestroySphereOverlap);

}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	SphereRadius = AttractSphere->GetScaledSphereRadius();
	
}

void AFPSBlackHole::OnDestroySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	AttractSphere->GetOverlappingComponents(OverlappingComponents);
	
	for(UPrimitiveComponent* Component : OverlappingComponents)
	{
		if(Component && Component->IsSimulatingPhysics())
		{
			Component->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}

}

