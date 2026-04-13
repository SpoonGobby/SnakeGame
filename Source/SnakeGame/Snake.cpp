// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"



// Sets default values
ASnake::ASnake()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Create and attach a static mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	//Create and attach the floating movement component
	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
}

// Called when the game starts or when spawned
void ASnake::BeginPlay()
{
	Super::BeginPlay();
	
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
	
	ASnake::MoveForward(MovementSpeed);
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
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASnake::MoveRight);
	}
}

//Handles forward/Backward movement
void ASnake::MoveForward(const FInputActionValue& Value)
{
	const float MovementValue = Value.Get<float>();
	FloatingMovement->AddInputVector(GetActorForwardVector() * MovementValue);
}

void ASnake::MoveRight(const FInputActionValue& Value)
{
	const float MovementValue = Value.Get<float>();
	if (FloatingMovement && MovementValue != 0.0f)
	{
		FloatingMovement->AddInputVector(GetActorRightVector() * MovementValue);
	}
}
