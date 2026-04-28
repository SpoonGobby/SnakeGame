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
#include "GameInst.h"

// Sets default values
ASnake::ASnake()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
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
	Score = 0;
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
	
	if (StartMoving)
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
	FloatingMovement->MaxSpeed = (1 + LengthSpeedMod * SnakeFollowerActor->InstancedMeshComponent->GetInstanceCount()) * 2000;
	const float MovementValue = MovementSpeed * GetWorld()->GetDeltaSeconds() * 80 * (1 + LengthSpeedMod * SnakeFollowerActor->InstancedMeshComponent->GetInstanceCount());
	FloatingMovement->AddInputVector(GetActorForwardVector() * MovementValue);
}

void ASnake::RotateLeftRight(const FInputActionValue& Value)
{
	if (!StartMoving)
		StartMoving = true;
	const float RotationValue = Value.Get<float>() * RotationSpeed * GetWorld()->GetDeltaSeconds() * 300 * (1 + LengthSpeedMod * SnakeFollowerActor->InstancedMeshComponent->GetInstanceCount());
	if (RotationValue != 0.0f)
	{
		AddActorLocalRotation(FRotator(0, RotationValue, 0));
	}
}

void ASnake::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (OtherActor != FoodActor)
		Die();
		
}

void ASnake::Die()
{
	UGameInst* GameInst = Cast<UGameInst>(GetWorld()->GetGameInstance());
	if (!GameInst->Multiplayer)
	{
		GameInst->LastScore = Score;
		GameInst->HighScore = std::max(Score, GameInst->HighScore);
	}
	else
    	GameInst->LastScore = Cast<APlayerController>(GetController())->GetLocalPlayer()->GetControllerId();
    	
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
