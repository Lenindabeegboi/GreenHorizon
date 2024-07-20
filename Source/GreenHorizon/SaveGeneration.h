// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGeneration.generated.h"

/**
 * 
 */
UCLASS()
class GREENHORIZON_API USaveGeneration : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	FName GenerationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	float CO2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 Economy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 Population;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 ResidentialArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	TMap<FVector, TSubclassOf<class AEstablishment>> ListOfPlacedEstablishments;  

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 Month;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info") 
	float TimeElapsed; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	float SustainabilityScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	float Length;



};
