// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"

void AMainPlayerController::ShowMainMenu_Implementation()
{
	if (WMainMenu)
	{
		if (!MainMenu) 
		{
			MainMenu = CreateWidget<UUserWidget>(this, WMainMenu);
		}
		if (MainMenu)
		{
			MainMenu->AddToViewport();
			MainMenu->SetVisibility(ESlateVisibility::Visible);

			FInputModeUIOnly UIOnly;
			SetInputMode(UIOnly);
			bShowMouseCursor = true;
		}
	}
}

void AMainPlayerController::HideMainMenu_Implementation()
{
	if (WMainMenu) 
	{
		if (MainMenu)
		{
			MainMenu->RemoveFromViewport(); 
			MainMenu->SetVisibility(ESlateVisibility::Hidden); 

		}
	}
}

void AMainPlayerController::ShowSelectGeneration_Implementation()
{
	if (WSelectGeneration)
	{
		
		if (!SelectGeneration)
		{
			SelectGeneration = CreateWidget<UUserWidget>(this, WSelectGeneration);
		}
		if (SelectGeneration)
		{
			SelectGeneration->AddToViewport();
			SelectGeneration->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AMainPlayerController::HideSelectGeneration_Implementation()
{
	if (WSelectGeneration)
	{
		if (SelectGeneration)
		{
			SelectGeneration->RemoveFromViewport();
			SelectGeneration->SetVisibility(ESlateVisibility::Hidden);

		}
	}
}

void AMainPlayerController::ShowNewGeneration_Implementation()
{
	if (WNewGeneration)
	{
		if (!NewGeneration)
		{
			NewGeneration = CreateWidget<UUserWidget>(this, WNewGeneration);
		}
		if (NewGeneration)
		{
			NewGeneration->AddToViewport();
			NewGeneration->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AMainPlayerController::HideNewGeneration_Implementation()
{
	if (WNewGeneration)
	{
		if (NewGeneration)
		{
			NewGeneration->RemoveFromViewport();
			NewGeneration->SetVisibility(ESlateVisibility::Hidden);

		}
	}
}

void AMainPlayerController::ShowSettings_Implementation()
{
	if (WSettings)
	{
		if (!Settings)
		{
			Settings = CreateWidget<UUserWidget>(this, WSettings);
		}
		if (Settings)
		{
			Settings->AddToViewport();
			Settings->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AMainPlayerController::HideSettings_Implementation()
{
	if (WSettings)
	{
		if (Settings)
		{
			Settings->RemoveFromViewport();
			Settings->SetVisibility(ESlateVisibility::Hidden);

		}
	}
}

void AMainPlayerController::CreateGeneration_Implementation()
{
	HideNewGeneration();
	HideSelectGeneration();
	UGameplayStatics::OpenLevel(GetWorld(), FName("City"));

	FInputModeGameAndUI GameAndUI;
	SetInputMode(GameAndUI);
	bShowMouseCursor = false;
	bGamePaused = false; 
}

void AMainPlayerController::BeginPlay()
{
	FString Current = GetWorld()->GetMapName();
	Current.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (Current == "MainMenu")
	{
		ShowMainMenu();
	}
}

void AMainPlayerController::ShowPauseMenu_Implementation()
{
	if (WPauseMenu)
	{
		if (!PauseMenu)
		{
			PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		}
		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Visible);
			bShowMouseCursor = true;
			bGamePaused = true;
		}
	}
}

void AMainPlayerController::HidePauseMenu_Implementation()
{
	if (WPauseMenu)
	{
		if (PauseMenu)
		{
			PauseMenu->RemoveFromViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
			bGamePaused = false; 
		}
	}
}
