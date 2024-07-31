// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GenerationInformation.generated.h"


UENUM(BlueprintType)			// Enumerator that describes how the player creates a generation, either by creating a completely new one (New), loading from a previous save instance of a generation (Load), or continuing the last generation played (Continue).
enum class ELoadingGameType : uint8
{
	None     = 0b0000 UMETA(DisplayName = "None"),
	New      = 0b0001 UMETA(DisplayName = "New"),
	Load     = 0b0010 UMETA(DisplayName = "Load"),
	Continue = 0b0011 UMETA(DisplayName = "Continue")
};

/**
 * 
 */
UCLASS()
class GREENHORIZON_API UGenerationInformation : public UPrimaryDataAsset		// Generation data asset class where its instance contains all relevant information about the current generation user plays at runtime. An instance of it also helps to translate its data into a save game instance when user saves a generation.
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Load Status")
	ELoadingGameType GameLoadStatus{ ELoadingGameType::None };				// Value that holds an enumerator that describes the way the player loads a generation. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	FName GenerationName;													// Name of the current generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	float CO2;																// CO2 emissions in the current generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 Economy;															// Economy (money) value in the current generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 Population;														// Population in the current generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info") 
	int32 ResidentialArea;													// Total residential area constituted by all residential property instances in the current generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 Month;															// Number of months passed in the current generation. Month updates by 1 per minute played in a generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info") 
	float TimeElapsed;														// The total time that has elapsed while playing the current generation. This is measured in seconds. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	float SustainabilityScore;												// The sustainability score based on all establishment instances placed in the current generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	FVector Position;														// The player's position in the current generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	FRotator Rotation;														// The player's rotation in the current generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	float Length;															// The player's camera distance in the current generation.
};
