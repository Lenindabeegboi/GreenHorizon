// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Establishment.h"
#include "CommercialEstablishment.generated.h"

/**
 * 
 */
UCLASS()
class GREENHORIZON_API ACommercialEstablishment : public AEstablishment			// Commercial class that inherits from the regular establishment class. Instances of this class help contribute to the economy in a generation. No special functions are added to this derived class, however, it helps pose as an identifier for instances of that class when generating wealth.
{
	GENERATED_BODY()
	
};
