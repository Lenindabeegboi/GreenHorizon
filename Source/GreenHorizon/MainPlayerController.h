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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void CreateGeneration();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowBuilder();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HideBuilder();

	bool bGamePaused;

protected:

	virtual void BeginPlay() override;

};
