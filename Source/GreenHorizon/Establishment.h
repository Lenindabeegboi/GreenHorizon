// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Establishment.generated.h"

UCLASS()
class GREENHORIZON_API AEstablishment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEstablishment();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sustainability")
	int32 Sustainability {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UMeshComponent* Structure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName EstablishmentName{ "Name" };


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
