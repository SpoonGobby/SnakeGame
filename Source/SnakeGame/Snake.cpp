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
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

float Time = 1;

// Sets default values
ASnake::ASnake()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	
	
	if (RootComponent)
	{
		ArrowComponent->SetupAttachment(RootComponent);
		SplineComponent->SetupAttachment(RootComponent);
	}
	
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
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		
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
	
	MoveForward();
	GenerateSplinePoint();
	SnakeFollowerActor->MoveInstancedMeshComponent();
	RotateArrow();
	
	
}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//I might re enable this later and add some stuff to make you move faster/slower when holding down W/S
		//EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ASnake::MoveForward);
		
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASnake::RotateLeftRight);
	}
}

//Handles forward/Backward movement
void ASnake::MoveForward()
{
		const float MovementValue = MovementSpeed * 0.8 * (1 + LengthSpeedMod * SnakeFollowerActor->InstancedMeshComponent->GetInstanceCount() * GetWorld()->GetDeltaSeconds());
		FloatingMovement->AddInputVector(GetActorForwardVector() * MovementValue);
}

void ASnake::RotateLeftRight(const FInputActionValue& Value)
{
	const float Rotationvalue = Value.Get<float>() * RotationSpeed * 200 * GetWorld()->GetDeltaSeconds();
	if (Rotationvalue != 0.0f)
	{
		AddActorLocalRotation(FRotator(0, Rotationvalue, 0));
	}
}

void ASnake::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Die();
}

void ASnake::Die()
{
	UGameplayStatics::OpenLevel(this, FName("MenuScreen"), true);
}

void ASnake::GenerateSplinePoint()
{
	FSplinePoint Point;
	
	Point.Position = GetActorLocation();
	Point.Rotation = GetActorRotation();
	SplineComponent->AddPoint(Point,true);
}

void ASnake::RotateArrow()
{
	ArrowComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FoodActor->GetActorLocation()));
}
