// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EstablishmentDataAsset.generated.h"

/**
 * 
 */

UCLASS()
class GREENHORIZON_API UEstablishmentDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName Name{ "Name" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description")
	FString Description{ "Description" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thumbnail")
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Establishment")
	TSubclassOf<class AEstablishment> Establishment;
	
};
