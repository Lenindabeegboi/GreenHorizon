// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */



UCLASS()
class GREENHORIZON_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WMainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WSelectGeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* SelectGeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WNewGeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* NewGeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* Settings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WPauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* PauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuilderWidget")
	TSubclassOf<UUserWidget> WBuilder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuilderWidget")
	UUserWidget* Builder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TSubclassOf<UUserWidget> WGameHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	UUserWidget* GameHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gnereation Info")
	class UMainGameInstance* GameInstance; 

	class USaveGeneration* LoadInstance;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowMainMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideMainMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowSelectGeneration();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideSelectGeneration();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowNewGeneration();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideNewGeneration();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowSettings();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideSettings();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowPauseMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HidePauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Generation")
	void CreateGeneration(FName GenerationName = NAME_None);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Generation")
	void QuitGeneration();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowBuilder();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideBuilder();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideGameHUD();

	void TranslatePlayerInformation(FVector& Position, FRotator& Rotation);
	void ChangeGenerationInfo(FVector Position, FRotator Rotation, float Length, float CO2, int32 Population, int32 Economy, int32 Month, float TimeElapsed, FName GenerationName, float SustainabilityScore, int32 ResidentialArea);
	void SaveGeneration();

	void SpawnEstatablishments(class FName GenerationName); 

	void MonthUpdate(); 
	void UpdateSustainability();
	void GenerateWealth(); 
	void UpdateCO2(); 

	void UpdatePopulation(); 

	void YearlyReward(); 

	bool bGamePaused;

protected:

	virtual void BeginPlay() override;

};
