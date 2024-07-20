// Fill out your copyright notice in the Description page of Project Settings.


#include "ExamplePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainPlayerController.h"
#include "BuilderComponent.h"
#include "Establishment.h"
#include "Blueprint/UserWidget.h"
#include "GenerationInformation.h"
#include "ResidentialProperty.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AExamplePawn::AExamplePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetupAttachment(SpringArmComponent, SpringArmComponent->SocketName);  
	BuilderComponent = CreateDefaultSubobject<UBuilderComponent>(TEXT("BuilderComponent"));
}

// Called when the game starts or when spawned
void AExamplePawn::BeginPlay()
{
	FString Current = GetWorld()->GetMapName(); 
	Current.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); 
	if (Current != "MainMenu") 
	{
		Super::BeginPlay(); 

		PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Emerald, TEXT("BEGIN")); 

		Velocity = FVector(0.f, 0.f, 0.f);
		Speed = 300.f;
		ZoomSpeed = 100.f;
		Position = PlayerController->GenerationInfo->Position;
		Rotation = PlayerController->GenerationInfo->Rotation;

		SpringArmComponent->TargetArmLength = PlayerController->GenerationInfo->Length;
		


		SetActorLocation(Position);
		SetActorRotation(Rotation);

		SelectedEstablishment, SelectedEstablishmentClass = nullptr;
	}
}

void AExamplePawn::MoveStraight(float Direction)
{
	Velocity.X = FMath::Clamp(Direction, -1.0f, 1.0f) * Speed * (SpringArmComponent->TargetArmLength / 3000);
	if (Velocity.X != 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Emerald, TEXT("MOVING"));
	}
}

void AExamplePawn::MoveSideways(float Direction)
{
	
	Velocity.Y = FMath::Clamp(Direction, -1.0f, 1.0f) * Speed * (SpringArmComponent->TargetArmLength / 3000);
	if (Velocity.Y != 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Emerald, TEXT("MOVING"));
	}
}

void AExamplePawn::Rotate(float Magnitude)
{
	RotationalVelocity.Yaw = FMath::Clamp(Magnitude, -1.0f, 1.0f);
	if (RotationalVelocity.Yaw != 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, TEXT("ROTATING"));
	}
}

void AExamplePawn::Zoom(float Magnitude)
{
	ZoomMovement = FMath::Clamp(Magnitude, -3.0f, 3.0f) * ZoomSpeed; 
	if (ZoomMovement != 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, TEXT("ZOOMING"));
	}
}

void AExamplePawn::PauseMenu()
{
	if (PlayerController) 
	{ 
		if (PlayerController->bGamePaused == false)
		{
			PlayerController->ShowPauseMenu();
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, TEXT("PAUSING"));
		}
		else
		{
			PlayerController->HidePauseMenu();
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, TEXT("RESUMING"));
		}
	}
}

void AExamplePawn::Place()
{
	if (SelectedEstablishmentClass && SelectedEstablishment)
	{
		if (!SelectedEstablishment->bConflictingWithOutsideEstablishment)
		{
			PlayerController->GenerationInfo->Economy -= SelectedEstablishment->Cost;

			AResidentialProperty* Residence = Cast<AResidentialProperty>(SelectedEstablishment);

			if (Residence) { PlayerController->GenerationInfo->ResidentialArea += Residence->ResidentialArea; }

			SelectedEstablishment = nullptr;
			SelectedEstablishmentClass = nullptr;

			PlayerController->UpdateSustainability();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, TEXT("CANNOT PLACE ESTABLISHMENT"));
		}
	}
}

void AExamplePawn::ToggleBuilder()
{
	if (PlayerController->Builder)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, TEXT("BUILDER"));
		if (PlayerController->Builder->IsVisible() == true)
		{
			PlayerController->HideBuilder();
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, TEXT("HIDING BUILDER"));
		}
		else
		{
			PlayerController->ShowBuilder();
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, TEXT("SHOWING BUILDER"));
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, TEXT("BUILDER TEST 2"));
}


// Called every frame
void AExamplePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerController->GenerationInfo->TimeElapsed += GetWorld()->GetDeltaSeconds();


	Velocity *= DeltaTime;
	AddActorLocalOffset(Velocity);
	Position = GetActorLocation();
	AddActorWorldRotation(RotationalVelocity);
	Rotation = GetActorRotation();
	SpringArmComponent->TargetArmLength += ZoomMovement;

	PlayerController->TranslatePlayerInformation(Position, Rotation);
	PlayerController->GenerationInfo->Length = SpringArmComponent->TargetArmLength;

	if (SelectedEstablishmentClass && SelectedEstablishment)
	{
		FHitResult HitResult; 
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitResult);
		SelectedEstablishment->SetActorLocation((FVector)HitResult.Location);
	}
	else if (SelectedEstablishmentClass)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitResult);
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Black, HitResult.Location.ToString()); 
		FActorSpawnParameters SpawnInfo;
		SelectedEstablishment = GetWorld()->SpawnActor<AEstablishment>(SelectedEstablishmentClass, (FVector)HitResult.Location, FRotator::ZeroRotator, SpawnInfo); 
	}

	if ((int32)PlayerController->GenerationInfo->TimeElapsed % 60 == 0 && (int32)PlayerController->GenerationInfo->TimeElapsed != 0) 
	{ 
		PlayerController->MonthUpdate(); 
		PlayerController->GenerationInfo->TimeElapsed += 1;
	}

}

// Called to bind functionality to input
void AExamplePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &AExamplePawn::MoveStraight);
	PlayerInputComponent->BindAxis("Right", this, &AExamplePawn::MoveSideways);
	PlayerInputComponent->BindAxis("Rotate", this, &AExamplePawn::Rotate);
	PlayerInputComponent->BindAxis("Zoom", this, &AExamplePawn::Zoom);

	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &AExamplePawn::PauseMenu);
	PlayerInputComponent->BindAction("PlaceEstablishment", IE_Pressed, this, &AExamplePawn::Place);
	PlayerInputComponent->BindAction("Builder", IE_Pressed, this, &AExamplePawn::ToggleBuilder); 
}
