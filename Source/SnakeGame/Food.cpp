// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "SnakeFollower.h"

// Sets default values
AFood::AFood()
{
	//Creates the mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	MeshComponent->SetupAttachment(DefaultSceneRoot);
	
	//Creates the box component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetBoxExtent(FVector(20.0f, 20.0f, 20.0f));
	BoxComponent->SetupAttachment(MeshComponent);
	BoxComponent->SetRelativeLocation(FVector(0.0f,0.0f, 100.0f));
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AFood::OnOverlapStart);
	
	MoveFood();
}
void AFood::OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASnake* Snake = Cast<ASnake>(OtherActor);
	ASnakeFollower* SnakeFollower = Cast<ASnakeFollower>(OtherActor);
	//UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(OtherComp);
	if (Snake != nullptr && OtherComp == Snake->MeshComponent)
	{
		if (!EatedFood)
		{
			EatedFood = true;
			if (Snake != nullptr && Snake->SnakeFollowerActor != nullptr && OtherComp == Snake->MeshComponent)
			{
				Snake->SnakeFollowerActor->Grow();
				MoveFood();
			}
		}
		else EatedFood = false;
	}
	else if (SnakeFollower == nullptr)
		MoveFood();
}

void AFood::MoveFood()
{
	int X = FMath::RandRange(BoundsExtents.X, -BoundsExtents.X);
	int Y = FMath::RandRange(BoundsExtents.Y, -BoundsExtents.Y);
	FVector Location = FVector(BoundsOrigin.X + X, BoundsOrigin.Y + Y, 50);
	TeleportTo(Location, this->GetActorRotation(), false);
}

