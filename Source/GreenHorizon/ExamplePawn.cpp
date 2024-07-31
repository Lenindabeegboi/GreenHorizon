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

	// Creation of all kwy components of the pawn class.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));						// Creation of the root component.
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));				// Creation of camera component. 
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));		// Creation of spring arm component
	SpringArmComponent->SetupAttachment(GetRootComponent());											// Binding of spring arm component to the root component
	CameraComponent->SetupAttachment(SpringArmComponent, SpringArmComponent->SocketName);				// Binding of camera component to the far end of the spring arm component
	BuilderComponent = CreateDefaultSubobject<UBuilderComponent>(TEXT("BuilderComponent"));				// Creation of builder component.
}

// Called when the game starts or when spawned. 
// This function resets when the game opens a new map in a new level.
void AExamplePawn::BeginPlay()
{
	// Getting the name of current map when game begins play.
	FString Current = GetWorld()->GetMapName(); 
	Current.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); 
	if (Current != "MainMenu")			// Block of code executes when the map is not the main menu map, but rather the city map.
	{
		Super::BeginPlay();				// Executes the 'BeginPlay' function of the regular pawn class that 'AExamplePawn' inherits from.

		
		PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));			// Class's player controller pointer reference points to the current player controller the game currently is using. Gets casted to 'AMainController' that allows extra functions to be executed.

		GameInstance = Cast<UMainGameInstance>(GetGameInstance());														// Class's game instance pointer reference points to the current game instance that the game is currently using. Gets casted to 'UMainGameInstance', to access the generation data asset pointer of 'UMainGameInstance'.

		// Defines Velocity, Speed, and ZoomSpeed to default values for every generation.
		Velocity = FVector(0.f, 0.f, 0.f);
		Speed = 300.f;
		ZoomSpeed = 100.f;
		// Sets the pawn instance's position and rotation information to the player physical information (position and rotation) from the generation data asset, referred to by the pawn's reference to the game instance. 
		Position = GameInstance->GenerationInfo->Position;
		Rotation = GameInstance->GenerationInfo->Rotation;

		// Translates the camera distance from the generation data asset, referred to by the pawn's reference to the game instance, to the length of the spring arm component.
		SpringArmComponent->TargetArmLength = GameInstance->GenerationInfo->Length;
		

		// Sets position and rotation information to the pawn instance's physical position and rotation on the world. 
		SetActorLocation(Position);
		SetActorRotation(Rotation);

		// Sets the references to new and current establishments, and selected establishment class to a null pointer when the pawn instance is spawned.
		SelectedEstablishment, SelectedEstablishmentClass, HighlightedEstablishment = nullptr;
	}
}

// Function that moves pawn instance forward and backwards.
void AExamplePawn::MoveStraight(float Direction)
{
	Velocity.X = FMath::Clamp(Direction, -1.0f, 1.0f) * Speed * (SpringArmComponent->TargetArmLength / 3000);
}

// Function that moves pawn instance left and right..
void AExamplePawn::MoveSideways(float Direction)
{
	
	Velocity.Y = FMath::Clamp(Direction, -1.0f, 1.0f) * Speed * (SpringArmComponent->TargetArmLength / 3000);
}

// Function that changes pawn instance's rotation.
void AExamplePawn::Rotate(float Magnitude)
{
	RotationalVelocity.Yaw = FMath::Clamp(Magnitude, -1.0f, 1.0f);
}

// Function that changes pawn instance's camera distance from generation. 
void AExamplePawn::Zoom(float Magnitude)
{
	ZoomMovement = FMath::Clamp(Magnitude, -3.0f, 3.0f) * ZoomSpeed; 
}

// Function that toggles the pause menu.
void AExamplePawn::PauseMenu()
{
	if (PlayerController)			// Checks if the pawn's references the player controller used in the game at runtime. If so the following block of code executes.
	{ 
		if (PlayerController->bGamePaused == false)				// If the game is paused, the pause menu disappears.
		{
			PlayerController->ShowPauseMenu();
		}
		else													// Else, if the game is not paused, the pause menu appears. 
		{
			PlayerController->HidePauseMenu();
		}
	}
}

// Function that places a new establishment on the generation.
void AExamplePawn::Place()
{
	if (SelectedEstablishmentClass && SelectedEstablishment)					// Checks if there is a selected establishment instance to be spawned, if so the following code executes.
	{
		if (!SelectedEstablishment->bConflictingWithOutsideEstablishment)							// If the new establishment is not overlapping with an existing establishment, the following block of code executes. 
		{
			GameInstance->GenerationInfo->Economy -= SelectedEstablishment->Cost;					// Deducts the 'Economy' stored by the generation data asset by the establishment instance's cost.

			AResidentialProperty* Residence = Cast<AResidentialProperty>(SelectedEstablishment);				// Casts the establishment to a 'AResidentialProperty' class.
			if (Residence) { GameInstance->GenerationInfo->ResidentialArea += Residence->ResidentialArea; }		// If the cast was successfull, meaning the establishment instance was a residential property, the establishment instance's residential area gets added to the total residential area stored in the generation data asset. 

			// Sets the establishment class and instance references to a null pointer, so the establishment instance referenced previously isn't manipulated any further.
			SelectedEstablishment = nullptr;
			SelectedEstablishmentClass = nullptr;

			PlayerController->UpdateSustainability();							// Updates the sustainability socre stored in the generation data asset after the new establishment instance has been placed on the generation.
		}
		// Otherwise, if the establishment instance is overlapping with an existing establishment, the 'error' sound plays, to notify the player that they can't place it.
		else			 
		{
			UGameplayStatics::PlaySound2D(SelectedEstablishment, ErrorSoundCue);
		}
	}
}

// Function that selects an already existing establishment
void AExamplePawn::Select()
{
	// If an establishment is already highlighted, then the establishment instance is deselected by changing the pointer reference of 'HighlightedEstablishment' to null pointer. 
	if (HighlightedEstablishment) 
	{
		HighlightedEstablishment = nullptr;
	}
	else if (!HighlightedEstablishment && !SelectedEstablishment)	// Otherwise, if no establishment is selected and there is no establishment instance that is to be spawned, the following code block executes
	{
		// Gets a hit result under the mouse cursor with the trace channel set to 'Camera'
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), true, HitResult); 
		HighlightedEstablishment = Cast<AEstablishment>(HitResult.GetActor());     // If the cursor's hit result detects a valid establishment instance, it gets selected by casting it to the 'AEstablishment' class and having it referenced by the 'HighlightedEstablishment' pointer. 
	}
}

// Function that deletes a selected establishment that is currently placed.
void AExamplePawn::Delete()
{
	// If an establishment instance is selected, the following code block executes. 
	if (HighlightedEstablishment) 
	{
		AResidentialProperty* ResidentialProperty = Cast<AResidentialProperty>(HighlightedEstablishment);	// Casts the establishment to a 'AResidentialProperty' class.
		if (ResidentialProperty) { PlayerController->GameInstance->GenerationInfo->ResidentialArea -= ResidentialProperty->ResidentialArea; }	// If the cast was successfull, meaning the establishment instance was a residential property, the establishment instance's residential area gets subtracted from the total residential area stored in the generation data asset. 
		HighlightedEstablishment->Destroy();			// Establishment instance gets deconstructed or destroyed. 
		PlayerController->UpdateSustainability();		// Updates sustainability after the establishment instance gets deleted from the generation. 
	}
}

// Function that duplicates a selected establishment that is currently placed.
void AExamplePawn::Duplicate()
{
	// If an establishment instance is selected, the following code block executes. 
	if (HighlightedEstablishment)
	{
		SelectedEstablishmentClass = HighlightedEstablishment->GetClass();      // Sets the selected establishment class to the class of the selected establishment instance that is currently placed.
		HighlightedEstablishment = nullptr;										// Deselects the selected establishment instance, by setting the 'HighlightedEstablishment' pointer, that was a reference to the establishment instance, to mull pointer. 
	}
}

// Function that closes and opens the builder widget on the player's screen. 
void AExamplePawn::ToggleBuilder()
{
	if (PlayerController->Builder)		// If the builder widget property of the player controller is valid, the following code executes.
	{
		// If the builder widget is visible on screen, it is hidden
		if (PlayerController->Builder->IsVisible() == true)	
		{
			PlayerController->HideBuilder();
		}
		// Otherwise, if the builder widget is not visible on screen, it is shown.
		else
		{
			PlayerController->ShowBuilder();
		}
	}
}


// Called every frame
void AExamplePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);				// Executes the 'Tick' function of the regular pawn class that 'AExamplePawn' inherits from.

	// Updates the elapsed time stored in the generation data asset by adding it to 'DeltaSeconds', which is the time in seconds between each frame. 
	GameInstance->GenerationInfo->TimeElapsed += GetWorld()->GetDeltaSeconds();

	Velocity *= DeltaTime;				// Multiplies pawn's velocity with DeltaTime, to make movement smoother rather than choppy.

	// Changes the pawn's physical position in the world with the velocity, which then gets assigned to the pawn's position variable.
	AddActorLocalOffset(Velocity);		
	Position = GetActorLocation();		
	// Changes the pawn's physical rotation in the world with the velocity, which then gets assigned to the pawn's rotation variable.
	AddActorWorldRotation(RotationalVelocity);
	Rotation = GetActorRotation();

	SpringArmComponent->TargetArmLength = FMath::Clamp(SpringArmComponent->TargetArmLength + ZoomMovement, 3000.f, 15000.f);	// Changes camera distance by zoom velocity. Also clamps the camera distance to a certain range, in case player tries going out of range. 

	PlayerController->TranslatePlayerInformation(Position, Rotation);			// Sets the generation data asset's player position and roataion values to the pawn's position and rotation variables respectively. 
	GameInstance->GenerationInfo->Length = SpringArmComponent->TargetArmLength;	// Sets the camera distance value in the generation data asset to the spring arm component's length.

	// If an establishment instance is newly spawned, but not placed yet, it follows the mouse cursor position but binds to the generation ground. 
	if (SelectedEstablishmentClass && SelectedEstablishment)
	{
		FHitResult HitResult; 
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitResult);
		SelectedEstablishment->SetActorLocation((FVector)HitResult.Location);
	}
	// Otherwise, if there is no establishment instance newly spawned, but an establishment class or child class has been selected (with reference to by 'SelectedEstablishmentClass'), an instance of it is spawned on the mouse cursor's position
	else if (SelectedEstablishmentClass)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitResult);
		FActorSpawnParameters SpawnInfo;
		SelectedEstablishment = GetWorld()->SpawnActor<AEstablishment>(SelectedEstablishmentClass, (FVector)HitResult.Location, FRotator::ZeroRotator, SpawnInfo); 
	}

	// If approximately a minute has elapsed in the generation total elapsed time, the month incremenets and potentially initiates in-game rewards for the player. 
	if ((int32)PlayerController->GameInstance->GenerationInfo->TimeElapsed % 60 == 0 && (int32)PlayerController->GameInstance->GenerationInfo->TimeElapsed != 0) 
	{ 
		PlayerController->MonthUpdate(); 
		GameInstance->GenerationInfo->TimeElapsed += 1;		// When the elapsed time is converted to an integer, the month would constantly update for around a second. To prevent this, an extra second is added to the generation's elapsed time value after the generation's month value updates. 
	}

}

// Called to bind functionality to input
void AExamplePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);			// Executes the 'SetupPlayerInputComponent' function of the regular pawn class that 'AExamplePawn' inherits from.

	// Maps all the movement functions to player axis inputs that are predefined in the 'Project Settings' of the engine editor. 
	PlayerInputComponent->BindAxis("Forward", this, &AExamplePawn::MoveStraight);		// W and S keys.
	PlayerInputComponent->BindAxis("Right", this, &AExamplePawn::MoveSideways);			// A and D keys.
	PlayerInputComponent->BindAxis("Rotate", this, &AExamplePawn::Rotate);				// Q and E keys.
	PlayerInputComponent->BindAxis("Zoom", this, &AExamplePawn::Zoom);					// Mouse scroll wheel.

	// Maps all the functions for establishment manipulation, and toggling of builder widget and pause menu, to player action inputs that are predefined in the 'Project Settings' of the engine editor. 
	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &AExamplePawn::PauseMenu);			// Escape key.
	PlayerInputComponent->BindAction("PlaceEstablishment", IE_Pressed, this, &AExamplePawn::Place);		// Left mouse click.
	PlayerInputComponent->BindAction("SelectEstablishment", IE_Pressed, this, &AExamplePawn::Select);	// Right mouse click.
	PlayerInputComponent->BindAction("Copy", IE_Pressed, this, &AExamplePawn::Duplicate);				// C key.
	PlayerInputComponent->BindAction("Delete", IE_Pressed, this, &AExamplePawn::Delete);				// X key.
	PlayerInputComponent->BindAction("Builder", IE_Pressed, this, &AExamplePawn::ToggleBuilder);		// Space bar.
}
