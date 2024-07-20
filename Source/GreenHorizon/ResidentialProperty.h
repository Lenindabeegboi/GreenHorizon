// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Establishment.h"
#include "ResidentialProperty.generated.h"

/**
 * 
 */
UCLASS()
class GREENHORIZON_API AResidentialProperty : public AEstablishment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area")
	int32 ResidentialArea{ 0 };
};
