// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Snake.generated.h"

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
	
	//Movement functions
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	
};
