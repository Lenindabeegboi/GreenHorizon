// Fill out your copyright notice in the Description page of Project Settings.

#include "Establishment.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEstablishment::AEstablishment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Structure = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
}

// Called when the game starts or when spawned
void AEstablishment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEstablishment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

