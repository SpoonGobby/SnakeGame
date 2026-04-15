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
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	MeshComponent->SetupAttachment(DefaultSceneRoot);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetBoxExtent(FVector(74.0f, 125.0f, 100.0f));
	BoxComponent->SetupAttachment(MeshComponent);
	BoxComponent->SetRelativeLocation(FVector(0.0f,0.0f, 100.0f));
	
	//Create and attach a static mesh component
	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	//RootComponent = MeshComponent;
	

}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AFood::OnOverlapStart);
	UE_LOG(LogTemp, Log, TEXT("Bello"));
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
		UE_LOG(LogTemp, Log, TEXT("Bello2"));
	}
}

