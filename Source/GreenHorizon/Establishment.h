// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Establishment.generated.h"

UCLASS(BlueprintType)
class GREENHORIZON_API AEstablishment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEstablishment();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sustainability")
	float Sustainability {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Structure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
	int32 Cost {1000};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	class UBoxComponent* BoxComponent;

	bool bConflictingWithOutsideEstablishment;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnEstablishmentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEstablishmentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
