// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GREENHORIZON_API UMainGameInstance : public UGameInstance     // Class for the game instance that the game uses. This class has the purpose of having a pointer reference to the generation data asset that stores in-generation data at runtime, for data manipulation. Like any game instance class, the 'UMainGameInstance' class should not reset when opening a new map in a new level  
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gnereation Info")
	class UGenerationInformation* GenerationInfo;		// Pointer reference to the generation data asset that holds in-generation data at runtime. 
};
