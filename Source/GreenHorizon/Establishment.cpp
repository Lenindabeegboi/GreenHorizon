// Fill out your copyright notice in the Description page of Project Settings.

#include "Establishment.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AEstablishment::AEstablishment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Structure = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Structure->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Structure->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);

	RootComponent = Structure;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	bConflictingWithOutsideEstablishment = false;
}

// Called when the game starts or when spawned
void AEstablishment::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEstablishment::OnEstablishmentBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AEstablishment::OnEstablishmentEndOverlap);
	
}

// Called every frame
void AEstablishment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEstablishment::OnEstablishmentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bConflictingWithOutsideEstablishment = true;
}

void AEstablishment::OnEstablishmentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bConflictingWithOutsideEstablishment = false; 
}

