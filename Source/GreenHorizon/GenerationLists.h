// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GenerationLists.generated.h"

/**
 * 
 */
UCLASS()
class GREENHORIZON_API UGenerationLists : public USaveGame     // A class for a save instance that keeps track of the names of all saved generations. This is to help manage loading saved generation slots.
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation List")
	TArray<FName> ListOfSavedGenerations;			// List that contains the names of all saved generations.
};
