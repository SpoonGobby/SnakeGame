// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeFollower.h"

// Sets default values
ASnakeFollower::ASnakeFollower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASnakeFollower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeFollower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASnakeFollower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

