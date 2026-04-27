// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "Food.generated.h"

class UBoxComponent;

UCLASS()
class SNAKEGAME_API AFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//Components
	UPROPERTY(VisibleAnywhere, category = "Components")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere, category = "Components")
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Components")
	TObjectPtr<UNiagaraSystem> NiagaraSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "SoundComponent")
	USoundBase* SoundOnEat;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "SpawnLocation")
	FVector BoundsOrigin;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "SpawnLocation")
	FVector BoundsExtents;
	
	
	UPROPERTY()
	USceneComponent* DefaultSceneRoot;
	
	UFUNCTION() void OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void MoveFood();
	
private:
	bool EatedFood;
	
	void CreateParticles();
};
