// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EstablishmentDataAsset.generated.h"

/**
 * 
 */

UCLASS()
class GREENHORIZON_API UEstablishmentDataAsset : public UPrimaryDataAsset		// A data asset class, where its instances store an establishment class, as well as relevant about the establishment class. Establishment data assets serve to translate the data into buttons on the builder widget, where the player can select them to spawn an instance of the corresponding establishment class stored, or hover over them to display the relevant information in a hoverbox.
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName Name{ "Name" };		// Name of the establishment class stored in the data asset instance

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description")
	FString Description{ "Description" };		// Brief description giving details about the establishment class. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thumbnail")
	UTexture2D* Thumbnail;		// Thumbnail image that provides a viusal preview of the establishment class's structure (static mesh component).

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Establishment")
	TSubclassOf<class AEstablishment> Establishment;		// Establishment class or child class that is to be spawned if player selects the button manifestation of the establishment data asset instance.
	
};
