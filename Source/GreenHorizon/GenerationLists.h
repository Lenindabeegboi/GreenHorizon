// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GenerationLists.generated.h"

/**
 * 
 */
UCLASS()
class GREENHORIZON_API UGenerationLists : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation List")
	TArray<FName> ListOfSavedGenerations;
};
