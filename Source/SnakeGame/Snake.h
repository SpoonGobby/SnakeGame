// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Components/SplineComponent.h"
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
	
	UPROPERTY(VisibleAnywhere, category = "Movement")
	UFloatingPawnMovement* FloatingMovement;
	
	UPROPERTY(VisibleAnywhere, category = "Components")
	USphereComponent* SphereComponent;
	
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
	const float MovementSpeed = 1;
	const float RotationSpeed = 2;
	
private:
	void GenerateSplinePoint();
	int SnakeLength;
	void MoveInstancedMeshComponent();
	bool EatedFood;
	
public:
	void EatFood();
	
};
