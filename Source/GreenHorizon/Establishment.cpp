// Fill out your copyright notice in the Description page of Project Settings.

#include "Establishment.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AEstablishment::AEstablishment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Creation of the establishment class's static mesh component and defining its collision responses; sets the static mesh componenent to only have a block response to any hit result with 'Camera' as the trace channel, whilst ignoring other channels.  
	Structure = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Structure->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Structure->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);

	
	RootComponent = Structure;			 // Setting the static mesh component as the class's root component.

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));			// Creation of the establishment class's box component.
	
	BoxComponent->SetupAttachment(GetRootComponent());		// Attaches the box component to the root component (static mesh component)

	// Sets the collision presets of the box component; setting the collision object type to 'World Static'.
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	// Sets the box component to only have an overlap response to actors that are classified with the 'World Static' collision object type.
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	bConflictingWithOutsideEstablishment = false;		// Setting the boolean property to false initially, since the establishment instance is not overlapping anything yet. 
}

// Called when the game starts or when spawned
void AEstablishment::BeginPlay()
{
	Super::BeginPlay();		// Executes the 'BeginPlay' function of the regular pawn class that 'AEstablishment' inherits from.

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEstablishment::OnEstablishmentBeginOverlap); // Box component binds overlap start event to 'OnEstablishmentBeginOverlap' function.
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AEstablishment::OnEstablishmentEndOverlap);	  // Box component binds overlap end event to 'OnEstablishmentEndOverlap' function.
	
}

// Called every frame
void AEstablishment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);		// Executes the 'Tick' function of the regular pawn class that 'AEstablishment' inherits from.

}

// Function that executes when the box component starts to detect an overlap.
void AEstablishment::OnEstablishmentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bConflictingWithOutsideEstablishment = true;		// Sets boolean property to true when box component starts overlapping.
}

// Function that executes once the box component stops overlapping.
void AEstablishment::OnEstablishmentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bConflictingWithOutsideEstablishment = false;		// Sets boolean property to false when box component finishes overlapping.
}

