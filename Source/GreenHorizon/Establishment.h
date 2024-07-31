// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Establishment.generated.h"

UCLASS(BlueprintType)
class GREENHORIZON_API AEstablishment : public AActor		// The class that allows the user to spawn instances of on a generation to impact in-generation values. This class also serves as an interface for custom establishment class, however 
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEstablishment();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sustainability")
	float Sustainability {0};					// A sustinability index of the establishment that contributes to changing a generation's sustainability score once an instance has been placed. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Structure;			// Static mesh component known as 'Structure' that makes the etablishment instance of the class visible to the player. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
	int32 Cost {1000};							// The cost of placing an instance of a class instance. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	class UBoxComponent* BoxComponent;			// Box component for the establishment class to detect any overlapping with other establishment instances. 

	bool bConflictingWithOutsideEstablishment;	// Boolean property that is based on whether the establishment instance overlaps with other establishment instances. Helps to determine whetehr establishment is overlapping or not.  
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function that executes when the box component starts to detect an overlap.
	UFUNCTION()
	void OnEstablishmentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function that executes once the box component stops overlapping.
	UFUNCTION()
	void OnEstablishmentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
