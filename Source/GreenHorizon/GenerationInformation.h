// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GenerationInformation.generated.h"


UENUM(BlueprintType)
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
class GREENHORIZON_API UGenerationInformation : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Load Status")
	ELoadingGameType GameLoadStatus{ ELoadingGameType::None };

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
