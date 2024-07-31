// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ExamplePawn.generated.h"


UCLASS()
class GREENHORIZON_API AExamplePawn : public APawn			// The pawn class that is the ayer for Green Horizon, inherits from the standard Pawn class.
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AExamplePawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") 
	class UCameraComponent* CameraComponent;							// Camera component for the pawn class, allows the player to view the world. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
	class USpringArmComponent* SpringArmComponent;						// Spring arm component for the pawn class; camera component binds to one end of the spring arm component, allowing to change distance between camera and player position 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController")
	class AMainPlayerController* PlayerController;						// Reference to the player controller used in the game at runtime.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Builder")
	class UBuilderComponent* BuilderComponent;							// Builder component for the pawn class to help the player manipulate the 'builder'.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Builder")
	TSubclassOf<class AEstablishment> SelectedEstablishmentClass;       // Reference to the 'AEstablishment' actor class or child class that is selected by the user for spawning an instance of on the generation. 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")		
	class USoundCue* ErrorSoundCue;										// Reference to sound cue that contains the 'Error' sound file.

	class UMainGameInstance* GameInstance;								// Reference to the game instance used by the game during runtime. 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// The velocities for player movement, rotation and zoom. Changes when player enters input, thus changing player physical position, rotation and camera distance from generation.
	FVector Velocity;
	FRotator RotationalVelocity;
	float ZoomMovement;

	// Stores the player's position and rotation on the generation. 
	FVector Position;
	FRotator Rotation;

	// The speed at which the player moves and rotates (Speed), and zooms (ZoomSpeed).
	float Speed;
	float ZoomSpeed;


	class AEstablishment* SelectedEstablishment;				// Reference to the establishment instance that gets spawned when player selects to spawn it.
	class AEstablishment* HighlightedEstablishment;				// Reference to the establishment instance (placed on the that generation) that the player selects.

	// Functions that define pawn's translation on the generation when taking input from player.
	void MoveStraight(float Direction);			// Forward and backwards movement.
	void MoveSideways(float Direction);			// Left and right movement.


	void Rotate(float Magnitude);				// Function that defines pawn's rotational change when taking input from player
	void Zoom(float Magnitude);					// Function that alters pawn's camera distance from the generation when taken input from player.

	void PauseMenu();							// Function that opens and closes pause menu page when taking player input

	// Functions that allow the user to spawn new establishment instances or manipulate current establishment instances. 
	void Place();								// Places a selected establishment instance on the generation. 
	void Select();								// Selects a placed establishment instance on the generation.
	void Delete();								// Deletes a selected establishment instance that is initially placed on the generation.
	void Duplicate();							// Duplicates a selected establishment instance that is initially placed on the generation.

	void ToggleBuilder();						// Function that opens and closes the builder when taking player input.


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
