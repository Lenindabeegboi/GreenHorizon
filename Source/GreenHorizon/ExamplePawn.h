// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ExamplePawn.generated.h"


UCLASS()
class GREENHORIZON_API AExamplePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AExamplePawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController")
	class AMainPlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Builder")
	class UBuilderComponent* BuilderComponent; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Builder")
	TSubclassOf<class AEstablishment> SelectedEstablishmentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* ErrorSoundCue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FVector Velocity;
	float Speed;
	FVector Position;
	FRotator Rotation;
	FRotator RotationalVelocity;
	float ZoomMovement;
	float ZoomSpeed;

	//bool bPlacedEstablishment;
	class AEstablishment* SelectedEstablishment; 
	class AEstablishment* HighlightedEstablishment;

	void MoveStraight(float Direction);
	void MoveSideways(float Direction);

	void Rotate(float Magnitude);
	void Zoom(float Magnitude);

	void PauseMenu();

	void Place();
	void Select();
	void Delete();
	void Duplicate();
	void ToggleBuilder(); 


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
