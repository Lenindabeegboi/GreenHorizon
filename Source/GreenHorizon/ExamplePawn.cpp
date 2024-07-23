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
#include "Sound/SoundCue.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h" 

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

		GameInstance = Cast<UMainGameInstance>(GetGameInstance());

		Velocity = FVector(0.f, 0.f, 0.f);
		Speed = 300.f;
		ZoomSpeed = 100.f;
		Position = GameInstance->GenerationInfo->Position;
		Rotation = GameInstance->GenerationInfo->Rotation;

		SpringArmComponent->TargetArmLength = GameInstance->GenerationInfo->Length;
		


		SetActorLocation(Position);
		SetActorRotation(Rotation);

		SelectedEstablishment, SelectedEstablishmentClass = nullptr;
	}
}

void AExamplePawn::MoveStraight(float Direction)
{
	Velocity.X = FMath::Clamp(Direction, -1.0f, 1.0f) * Speed * (SpringArmComponent->TargetArmLength / 3000);
}

void AExamplePawn::MoveSideways(float Direction)
{
	
	Velocity.Y = FMath::Clamp(Direction, -1.0f, 1.0f) * Speed * (SpringArmComponent->TargetArmLength / 3000);
}

void AExamplePawn::Rotate(float Magnitude)
{
	RotationalVelocity.Yaw = FMath::Clamp(Magnitude, -1.0f, 1.0f);
}

void AExamplePawn::Zoom(float Magnitude)
{
	ZoomMovement = FMath::Clamp(Magnitude, -3.0f, 3.0f) * ZoomSpeed; 
}

void AExamplePawn::PauseMenu()
{
	if (PlayerController) 
	{ 
		if (PlayerController->bGamePaused == false)
		{
			PlayerController->ShowPauseMenu();
		}
		else
		{
			PlayerController->HidePauseMenu();
		}
	}
}

void AExamplePawn::Place()
{
	if (SelectedEstablishmentClass && SelectedEstablishment)
	{
		if (!SelectedEstablishment->bConflictingWithOutsideEstablishment) 
		{
			GameInstance->GenerationInfo->Economy -= SelectedEstablishment->Cost; 

			AResidentialProperty* Residence = Cast<AResidentialProperty>(SelectedEstablishment);

			if (Residence) { GameInstance->GenerationInfo->ResidentialArea += Residence->ResidentialArea; }

			SelectedEstablishment = nullptr;
			SelectedEstablishmentClass = nullptr;

			PlayerController->UpdateSustainability();
		}
		else
		{
			UGameplayStatics::PlaySound2D(SelectedEstablishment, ErrorSoundCue);
		}
	}
}

void AExamplePawn::Select()
{
	if (HighlightedEstablishment) 
	{
		HighlightedEstablishment = nullptr;
	}
	else
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), true, HitResult); 
		HighlightedEstablishment = Cast<AEstablishment>(HitResult.GetActor());
	}
}

void AExamplePawn::Delete()
{
	if (HighlightedEstablishment) 
	{ 
		HighlightedEstablishment->Destroy(); 
		PlayerController->UpdateSustainability();
	}
}

void AExamplePawn::Duplicate()
{
	if (HighlightedEstablishment)
	{
		SelectedEstablishmentClass = HighlightedEstablishment->GetClass();
		HighlightedEstablishment = nullptr;
	}
}

void AExamplePawn::ToggleBuilder()
{
	if (PlayerController->Builder)
	{
		if (PlayerController->Builder->IsVisible() == true)
		{
			PlayerController->HideBuilder();
		}
		else
		{
			PlayerController->ShowBuilder();
		}
	}
}


// Called every frame
void AExamplePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameInstance->GenerationInfo->TimeElapsed += GetWorld()->GetDeltaSeconds();


	Velocity *= DeltaTime;
	AddActorLocalOffset(Velocity);
	Position = GetActorLocation();
	AddActorWorldRotation(RotationalVelocity);
	Rotation = GetActorRotation();
	SpringArmComponent->TargetArmLength = FMath::Clamp(SpringArmComponent->TargetArmLength + ZoomMovement, 3000.f, 15000.f);

	PlayerController->TranslatePlayerInformation(Position, Rotation);
	GameInstance->GenerationInfo->Length = SpringArmComponent->TargetArmLength;

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
		FActorSpawnParameters SpawnInfo;
		SelectedEstablishment = GetWorld()->SpawnActor<AEstablishment>(SelectedEstablishmentClass, (FVector)HitResult.Location, FRotator::ZeroRotator, SpawnInfo); 
	}

	if ((int32)PlayerController->GameInstance->GenerationInfo->TimeElapsed % 60 == 0 && (int32)PlayerController->GameInstance->GenerationInfo->TimeElapsed != 0) 
	{ 
		PlayerController->MonthUpdate(); 
		GameInstance->GenerationInfo->TimeElapsed += 1;
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
	PlayerInputComponent->BindAction("SelectEstablishment", IE_Pressed, this, &AExamplePawn::Select);
	PlayerInputComponent->BindAction("Copy", IE_Pressed, this, &AExamplePawn::Duplicate);
	PlayerInputComponent->BindAction("Delete", IE_Pressed, this, &AExamplePawn::Delete);
	PlayerInputComponent->BindAction("Builder", IE_Pressed, this, &AExamplePawn::ToggleBuilder); 
}
