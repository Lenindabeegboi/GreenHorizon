// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */



UCLASS()
class GREENHORIZON_API AMainPlayerController : public APlayerController     // Player controller class that the game uses for manipulation of UI, manipulating generation information and saving generations. 
{
	GENERATED_BODY()

public: 
	// The widget class and reference to widget instance of the main menu respectively.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WMainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* MainMenu;

	// The widget class and reference to widget instance of the 'Select Generation' page respectively.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WSelectGeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* SelectGeneration;

	// The widget class and reference to widget instance of the 'New Generation' page respectively.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WNewGeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* NewGeneration;

	// The widget class and reference to widget instance of the 'Settings' page respectively.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* Settings;

	// The widget class and reference to widget instance of the pause menu respectively.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WPauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* PauseMenu;

	// The widget class and reference to widget instance of the builder respectively.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuilderWidget")
	TSubclassOf<UUserWidget> WBuilder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuilderWidget")
	UUserWidget* Builder;

	// The widget class and reference to widget instance of the HUD overlay whenever loading or creating a generation respectively.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TSubclassOf<UUserWidget> WGameHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	UUserWidget* GameHUD;
	



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gnereation Info")
	class UMainGameInstance* GameInstance;				// Reference to the game instance used by the game. 

	class USaveGeneration* LoadInstance;				// Reference to a saved slot of a generation.

	
	// Functions to show and hide the main menu respectively.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowMainMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideMainMenu();

	// Functions to show and hide the 'Select Generation' page respectively.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowSelectGeneration();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideSelectGeneration();

	// Functions to show and hide the 'New Generation' page respectively.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowNewGeneration();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideNewGeneration();

	// Functions to show and hide the 'Settings' page respectively.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowSettings();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideSettings();

	// Functions to show and hide the pause menu respectively.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowPauseMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HidePauseMenu();

	// Functions to show and hide the builder respectively.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowBuilder();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD") 
	void HideBuilder();

	// Functions to show and hide the HUD overlay whenever loading or creating a generation respectively.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD") 
	void HideGameHUD();




	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Generation")
	void CreateGeneration(FName GenerationName = NAME_None);					// Function for creating a generation or loading a generation from a save slot.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Generation")
	void QuitGeneration();														// Function for quitting a generation.



	void TranslatePlayerInformation(FVector& Position, FRotator& Rotation);			// Function that changes the position and rotation values of the generation data asset. 
	void ChangeGenerationInfo(FVector Position, FRotator Rotation, float Length, float CO2, int32 Population, int32 Economy, int32 Month, float TimeElapsed, FName GenerationName, float SustainabilityScore, int32 ResidentialArea);	// Function that changes the information of the generation data asset. 
	void SaveGeneration();			// Function that saves generation data asset information to a new or exisiting save instance. 

	void SpawnEstatablishments(class FName GenerationName);			// Spawns all establishments from a saved slot of a generation.

	void MonthUpdate();					// Updates the generation month value and gives the players rewards based on their changes to the generation.
	void UpdateSustainability();		// Updates the sustainability score of the generation based on all establishment instances placed. 
	void GenerateWealth();				// Adds more wealth to the generation's economy value based on the number of commercial establishments placed.
	void UpdateCO2();					// Updates to CO2 value in the generation. 
	void UpdatePopulation();			// Updates the population in the generation.

	void YearlyReward();				// Gives the user special rewards. This function only executes for every 12 minutes elapsed in the generation. 

	bool bGamePaused;					// Boolean value that checks if the pause menu is visible. 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
