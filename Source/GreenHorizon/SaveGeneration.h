// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGeneration.generated.h"

/**
 * 
 */
UCLASS()
class GREENHORIZON_API USaveGeneration : public USaveGame			// This class allows the player to translate generation information into instances of the class and save them into slots in the player's local storage.  
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	FName GenerationName;														// Name of the saved generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	float CO2;																	// CO2 emissions in the saved generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 Economy;																// Economy (money) value in the saved generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 Population;															// Population in the saved generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 ResidentialArea;														// Total residential area constituted by all residential property instances in the saved generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	TMap<FVector, TSubclassOf<class AEstablishment>> ListOfPlacedEstablishments;  // 'TMap' container that stores 'TPair' elements that map a unique FVector position as a key to an establishment class or child class as a value. Helps to keep track of the types of establishment instances and their positions on the saved generation, to help spawn the establishment instances when loading a saved instance.  

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int32 Month;																// Number of months passed in the saved generation. Month updates by 1 per minute played in a generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info") 
	float TimeElapsed;															// The total time that has elapsed while playing the saved generation. This is measured in seconds. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	float SustainabilityScore;													// The sustainability score based on all establishment instances placed in the saved generation based.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	FVector Position;															// The player's position in the saved generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	FRotator Rotation;															// The player's rotation in the saved generation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	float Length;																// The player's camera distance in the saved generation.



};
