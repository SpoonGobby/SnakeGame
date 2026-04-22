// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"

float Time = 1;

// Sets default values
ASnake::ASnake()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Create and attach a static mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	InstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	
	RootComponent = MeshComponent;
	
	
	if (RootComponent)
	{
		SplineComponent->SetupAttachment(RootComponent);
		InstancedMeshComponent->SetupAttachment(RootComponent);
	}
	
	//Create and attach the floating movement component
	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
}

// Called when the game starts or when spawned
void ASnake::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->OnComponentHit.AddDynamic(this, &ASnake::OnComponentHit);
	
	//Get the player controller
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		//Get the enhanced input subsystem from the local player
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		
		//Add the input mapping context to the subsystem
		if (Subsystem && InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext,0);
		}
	}
	
}

// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Time = DeltaTime;
	
	MoveForward(DeltaTime);
	GenerateSplinePoint();
	MoveInstancedMeshComponent();
}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind MoveForward action
		//I might re enable this later and add some stuff to make you move faster/slower when holding down W/S
		//EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ASnake::MoveForward);
		
		// Bind MoveRight Action
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASnake::RotateLeftRight);
	}
}

//Handles forward/Backward movement
void ASnake::MoveForward(const FInputActionValue& Value)
{
		const float MovementValue = MovementSpeed * 50 / Time;
		FloatingMovement->AddInputVector(GetActorForwardVector() * MovementValue);
}

void ASnake::RotateLeftRight(const FInputActionValue& Value)
{
	const float Rotationvalue = Value.Get<float>() * RotationSpeed * 5;
	if (FloatingMovement && Rotationvalue != 0.0f)
	{
		AddActorLocalRotation(FRotator(0, Rotationvalue, 0));
	}
}

void ASnake::EatFood()
{
	if (EatedFood)
	{
		InstancedMeshComponent->AddInstance(FTransform(GetActorLocation()),true);
		EatedFood = false;
	}
	else EatedFood = true;
}

void ASnake::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//works
}

void ASnake::GenerateSplinePoint()
{
	FSplinePoint Point;
	
	Point.Position = GetActorLocation();
	Point.Rotation = GetActorRotation();
	SplineComponent->AddPoint(Point,true);
}

void ASnake::MoveInstancedMeshComponent()
{
	for (int i = 0; i < InstancedMeshComponent->GetInstanceCount(); i++)
	{
		float Distance = (i+1)*100;
		FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
		FTransform Transform(Location);
		InstancedMeshComponent->UpdateInstanceTransform(i, Transform, true);
	}
}
