// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Snake.h"

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFood::OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASnake* Snake = (ASnake*)OtherActor;
	if (Snake != nullptr)
	{
		
		Snake->EatFood();
	}
}

