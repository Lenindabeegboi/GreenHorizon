// Fill out your copyright notice in the Description page of Project Settings.


#include "ExamplePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainPlayerController.h"
#include "BuilderComponent.h"
#include "Blueprint/UserWidget.h"
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
		Velocity = FVector(0.f, 0.f, 0.f); 
		Speed = 300.f;
		Position = FVector(0.f, 0.f, 134.f);
		Rotation = FRotator(0.f, 45.f, 0.f);

		SpringArmComponent->TargetArmLength = 3000.f;

		SetActorLocation(Position);
		SetActorRotation(Rotation);

		PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Emerald, TEXT("BEGIN")); 

	}
}

void AExamplePawn::MoveStraight(float Direction)
{
	Velocity.X = FMath::Clamp(Direction, -1.0f, 1.0f) * Speed;
	if (Velocity.X != 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Emerald, TEXT("MOVING"));
	}
}

void AExamplePawn::MoveSideways(float Direction)
{
	
	Velocity.Y = FMath::Clamp(Direction, -1.0f, 1.0f) * Speed;
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
	ZoomMovement = FMath::Clamp(Magnitude, -3.0f, 3.0f);
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

	Velocity *= DeltaTime;
	AddActorLocalOffset(Velocity);
	Position = GetActorLocation();
	AddActorWorldRotation(RotationalVelocity);
	Rotation = GetActorRotation();
	SpringArmComponent->TargetArmLength += ZoomMovement;
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
