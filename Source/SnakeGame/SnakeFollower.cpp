// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeFollower.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
ASnakeFollower::ASnakeFollower()
{
	InstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	RootComponent = InstancedMeshComponent;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	
	if (RootComponent)
	{
		SplineComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ASnakeFollower::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASnakeFollower::Grow()
{
	InstancedMeshComponent->AddInstance(FTransform(GetActorLocation()),true);
}

void ASnakeFollower::MoveInstancedMeshComponent()
{
	for (int i = 0; i < InstancedMeshComponent->GetInstanceCount(); i++)
	{
		float Distance = (i+1)*100;
		FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
		FTransform Transform(Location);
		InstancedMeshComponent->UpdateInstanceTransform(i, Transform, true);
	}
}
