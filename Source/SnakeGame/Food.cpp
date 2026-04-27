// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"

#include "Snake.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "SnakeFollower.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFood::AFood()
{
	//Creates the mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = MeshComponent;
	
	//Creates the box component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetBoxExtent(FVector(20.0f, 20.0f, 20.0f));
	BoxComponent->SetRelativeLocation(FVector(0.0f,0.0f, 100.0f));
	
	if (RootComponent)
	{
		BoxComponent->SetupAttachment(RootComponent);
	}
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
	if (Snake != nullptr && OtherComp == Snake->MeshComponent)
	{
		if (!EatedFood)
		{
			EatedFood = true;
			if (Snake != nullptr && Snake->SnakeFollowerActor != nullptr && OtherComp == Snake->MeshComponent)
			{
				Snake->Score++;
				Snake->SnakeFollowerActor->Grow();
				CreateParticles();
				UGameplayStatics::PlaySoundAtLocation(this, SoundOnEat, GetActorLocation());
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

void AFood::CreateParticles()
{
	if (NiagaraSystem) 
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraSystem,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.f), // Scale
			true,         // Auto-destroy when finished
			true,
			ENCPoolMethod::None          // Auto-activate
		);
	}
}
