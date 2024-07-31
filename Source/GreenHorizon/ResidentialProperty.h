// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Establishment.h"
#include "ResidentialProperty.generated.h"

/**
 * 
 */
UCLASS()
class GREENHORIZON_API AResidentialProperty : public AEstablishment			// Residential property class that inherits from the regular establishment class. Contributes to population change by adding to the total residential area of a generation.
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area")
	int32 ResidentialArea{ 0 };			// Residential area value of the class that can be added to a generation's total residential area to help populatio growth once an instance of the residential area class has been placed on the generation.
};
