// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInst.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEGAME_API UGameInst : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
	int Difficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
	int HighScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
	int LastScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
	bool Multiplayer;
};
