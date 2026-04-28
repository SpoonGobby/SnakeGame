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
	PrimaryActorTick.bCanEverTick = true;
	
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
			Snake->Score++;
			Snake->SnakeFollowerActor->Grow();
			CreateParticles();
			UGameplayStatics::PlaySoundAtLocation(this, SoundOnEat, GetActorLocation());
			MoveFood();
		}
		else EatedFood = false;
	}
	else if (SnakeFollower == nullptr)
		MoveFood();
}

void AFood::MoveFood()
{
	for (int i = 0; i < 500; i++)
	{
		int X = FMath::RandRange(BoundsExtents.X, -BoundsExtents.X);
		int Y = FMath::RandRange(BoundsExtents.Y, -BoundsExtents.Y);
		FVector NewLocation = FVector(BoundsOrigin.X + X, BoundsOrigin.Y + Y, 55);

		// Define the collision shape (use the same size as your food's collider)
		FVector Origin;
		FVector BoxExtent;
		GetActorBounds(false, Origin, BoxExtent);
		FCollisionShape MyShape = FCollisionShape::MakeBox(BoxExtent); 
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		// Check if anything is at that location
		bool bHit = GetWorld()->OverlapAnyTestByChannel(
			NewLocation, 
			FQuat::Identity, 
			ECC_WorldStatic, // Or ECC_Visibility/ECC_OverlapAll depending on your walls
			MyShape, 
			Params
		);

		if (!bHit)
		{
			SetActorLocation(NewLocation);
			break;
		}
	}
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
