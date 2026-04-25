// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "Snake.generated.h"

class USphereComponent;

UCLASS()
class SNAKEGAME_API ASnake : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnake();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Components
	UPROPERTY(VisibleAnywhere, category = "Components")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UInstancedStaticMeshComponent* InstancedMeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USplineComponent* SplineComponent;
	
	UPROPERTY(VisibleAnywhere, category = "Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, category = "Components")
	UArrowComponent* ArrowComponent;
	
	UPROPERTY(VisibleAnywhere, category = "Movement")
	UFloatingPawnMovement* FloatingMovement;
	
	//Input actions for enhanced input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveForwardAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveRightAction;
	
	//Input mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;
	
	//Collision check
	UFUNCTION() void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	//Movement functions
	void MoveForward(const FInputActionValue& Value);
	void RotateLeftRight(const FInputActionValue& Value);
	
private:
	void GenerateSplinePoint();
	int SnakeLength;
	void MoveInstancedMeshComponent();
	bool EatedFood;
	
	//Movement Speeds
	const float MovementSpeed = 1;
	const float RotationSpeed = 2;
	const float LengthSpeedMod = 1.1;
	
	//Arrow
	void RotateArrow();
	
	
public:
	void EatFood();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "SpawnLocation")
	AActor* FoodActor;
};
