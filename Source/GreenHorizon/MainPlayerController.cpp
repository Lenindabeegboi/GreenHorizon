// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "GenerationInformation.h"
#include "SaveGeneration.h"
#include "Establishment.h"
#include "GenerationLists.h"
#include "CommercialEstablishment.h"
#include "MainGameInstance.h"
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
		}
	}
}

void AMainPlayerController::HideMainMenu_Implementation()
{

	if (MainMenu)
	{
		MainMenu->RemoveFromViewport(); 
		MainMenu->SetVisibility(ESlateVisibility::Hidden); 
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

	if (SelectGeneration)
	{
		SelectGeneration->RemoveFromViewport();
		SelectGeneration->SetVisibility(ESlateVisibility::Hidden);
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

	if (NewGeneration)
	{
		NewGeneration->RemoveFromViewport();
		NewGeneration->SetVisibility(ESlateVisibility::Hidden);
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

	if (Settings)
	{
		Settings->RemoveFromViewport();
		Settings->SetVisibility(ESlateVisibility::Hidden);

	}
}


void AMainPlayerController::ChangeGenerationInfo(FVector Position, FRotator Rotation, float Length, float CO2, int32 Population, int32 Economy, int32 Month, float TimeElapsed, FName GenerationName, float SustainabilityScore, int32 ResidentialArea) 
{
	GameInstance->GenerationInfo->GenerationName = GenerationName;

	GameInstance->GenerationInfo->CO2 = CO2; 
	GameInstance->GenerationInfo->Economy = Economy; 
	GameInstance->GenerationInfo->Population = Population; 
	GameInstance->GenerationInfo->ResidentialArea = ResidentialArea;
	GameInstance->GenerationInfo->SustainabilityScore = SustainabilityScore;

	GameInstance->GenerationInfo->Month = Month;
	GameInstance->GenerationInfo->TimeElapsed = TimeElapsed; 

	GameInstance->GenerationInfo->Position = Position; 
	GameInstance->GenerationInfo->Rotation = Rotation;
	GameInstance->GenerationInfo->Length = Length; 
}

void AMainPlayerController::SpawnEstatablishments(FName GenerationName)
{
	USaveGeneration* SaveInstance = Cast<USaveGeneration>(UGameplayStatics::LoadGameFromSlot(GenerationName.ToString(), 0));


	if (SaveInstance->ListOfPlacedEstablishments.Num() != 0)
	{
		FActorSpawnParameters SpawnInfo; 
		for (TPair<FVector, TSubclassOf<AEstablishment>>& Establishment : SaveInstance->ListOfPlacedEstablishments) 
		{

			GetWorld()->SpawnActor<AEstablishment>(Establishment.Value, Establishment.Key, FRotator::ZeroRotator, SpawnInfo);
		}
	} 
}

void AMainPlayerController::UpdateSustainability()
{
	float TotalSustainability = 0.f;

	TArray<AActor*> Establishments = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEstablishment::StaticClass(), Establishments);

	for (AActor* Actor : Establishments)
	{
		AEstablishment* Establishment = Cast<AEstablishment>(Actor);
		if (Establishment)
		{
			TotalSustainability += Establishment->Sustainability;
		}
	}

	GameInstance->GenerationInfo->SustainabilityScore = TotalSustainability / Establishments.Num();
}

void AMainPlayerController::GenerateWealth()
{	
	int32 Counter = 0;

	TArray<AActor*> CommercialEstablishments = TArray<AActor*>(); 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACommercialEstablishment::StaticClass(), CommercialEstablishments);

	for (AActor* Actor : CommercialEstablishments)
	{
		ACommercialEstablishment* CommercialEstablishment = Cast<ACommercialEstablishment>(Actor);
		if (CommercialEstablishment)
		{
			Counter += 1;
		}
	}

	GameInstance->GenerationInfo->Economy += Counter * int32(GameInstance->GenerationInfo->SustainabilityScore * pow(2.f, (FString::FromInt(GameInstance->GenerationInfo->Population)).Len())); 
}

void AMainPlayerController::UpdateCO2()
{
	GameInstance->GenerationInfo->CO2 += int32((GameInstance->GenerationInfo->Population / GameInstance->GenerationInfo->SustainabilityScore) * 1500) / 100;
}

void AMainPlayerController::UpdatePopulation()
{
	int32 ProjectedPopulation = 0;
	if (GameInstance->GenerationInfo->SustainabilityScore >= 5)
	{
		ProjectedPopulation = int32((GameInstance->GenerationInfo->ResidentialArea / 20) * (GameInstance->GenerationInfo->SustainabilityScore / 3));
	}
	else
	{
		ProjectedPopulation = int32((GameInstance->GenerationInfo->ResidentialArea / 20) / (5 - GameInstance->GenerationInfo->SustainabilityScore));
	}

	if (GameInstance->GenerationInfo->Population != ProjectedPopulation) { GameInstance->GenerationInfo->Population = ProjectedPopulation; }
}

void AMainPlayerController::YearlyReward()
{
	GameInstance->GenerationInfo->Economy += int32((GameInstance->GenerationInfo->Population / (FString::FromInt(GameInstance->GenerationInfo->Economy)).Len()) * GameInstance->GenerationInfo->SustainabilityScore * 10);
}

void AMainPlayerController::MonthUpdate()
{
	GameInstance->GenerationInfo->Month += 1;
	UpdatePopulation();
	UpdateCO2();
	GenerateWealth();
	if (GameInstance->GenerationInfo->Month % 12 == 0 && GameInstance->GenerationInfo->Month != 0) 
	{ 
		YearlyReward(); 
	}
}

void AMainPlayerController::CreateGeneration_Implementation(FName GenerationName) 
{
	
	if (GameInstance->GenerationInfo->GameLoadStatus == ELoadingGameType::New)
	{
		ChangeGenerationInfo(FVector(0.f, 0.f, 300.f), FRotator(0.f, 0.f, 0.f), 3000.f, 400.f, 0, 10000000, 0, 0.f, GenerationName, 10.f, 0.f);
		LoadInstance = nullptr;
	}
	else if (GameInstance->GenerationInfo->GameLoadStatus == ELoadingGameType::Continue)
	{
		UGenerationLists* GenerationList = Cast<UGenerationLists>(UGameplayStatics::LoadGameFromSlot("@---GENERATION---LIST---@", 0));
		LoadInstance = Cast<USaveGeneration>(UGameplayStatics::LoadGameFromSlot(GenerationList->ListOfSavedGenerations.Last().ToString(), 0));
		ChangeGenerationInfo(LoadInstance->Position, LoadInstance->Rotation, LoadInstance->Length, LoadInstance->CO2, LoadInstance->Population, LoadInstance->Economy, LoadInstance->Month, LoadInstance->TimeElapsed, LoadInstance->GenerationName, LoadInstance->SustainabilityScore, LoadInstance->ResidentialArea);
	}
	else if (GameInstance->GenerationInfo->GameLoadStatus == ELoadingGameType::Load)
	{
		UGenerationLists* GenerationList = Cast<UGenerationLists>(UGameplayStatics::LoadGameFromSlot("@---GENERATION---LIST---@", 0));
		LoadInstance = Cast<USaveGeneration>(UGameplayStatics::LoadGameFromSlot(GenerationName.ToString(), 0));
		ChangeGenerationInfo(LoadInstance->Position, LoadInstance->Rotation, LoadInstance->Length, LoadInstance->CO2, LoadInstance->Population, LoadInstance->Economy, LoadInstance->Month, LoadInstance->TimeElapsed, LoadInstance->GenerationName, LoadInstance->SustainabilityScore, LoadInstance->ResidentialArea);
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("City"));
}

void AMainPlayerController::ShowGameHUD_Implementation()
{
	if (WGameHUD)
	{
		if (!GameHUD)
		{
			GameHUD = CreateWidget<UUserWidget>(this, WGameHUD);
		}
		if (GameHUD)
		{
			GameHUD->AddToViewport();
			GameHUD->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AMainPlayerController::HideGameHUD_Implementation() 
{
	if (GameHUD) 
	{
		GameHUD->RemoveFromViewport();
		GameHUD->SetVisibility(ESlateVisibility::Hidden);

	}
}

void AMainPlayerController::TranslatePlayerInformation(FVector& Position, FRotator& Rotation)
{
	GameInstance->GenerationInfo->Position = Position;
	GameInstance->GenerationInfo->Rotation = Rotation;
}

void AMainPlayerController::SaveGeneration()
{
	USaveGeneration* SaveInstance = Cast<USaveGeneration>(UGameplayStatics::CreateSaveGameObject(USaveGeneration::StaticClass()));
	SaveInstance->Position = GameInstance->GenerationInfo->Position;
	SaveInstance->Rotation = GameInstance->GenerationInfo->Rotation;
	SaveInstance->Length = GameInstance->GenerationInfo->Length;
	 
	SaveInstance->Population = GameInstance->GenerationInfo->Population; 
	SaveInstance->ResidentialArea = GameInstance->GenerationInfo->ResidentialArea;
	SaveInstance->CO2 = GameInstance->GenerationInfo->CO2; 
	SaveInstance->Economy = GameInstance->GenerationInfo->Economy;
	SaveInstance->SustainabilityScore = GameInstance->GenerationInfo->SustainabilityScore;

	SaveInstance->Month = GameInstance->GenerationInfo->Month;
	SaveInstance->TimeElapsed = GameInstance->GenerationInfo->TimeElapsed;

	SaveInstance->GenerationName = GameInstance->GenerationInfo->GenerationName; 

	TArray<AActor*> Establishments = TArray<AActor*>(); 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEstablishment::StaticClass(), Establishments); 

	for (AActor* &Actor : Establishments) 
	{ 
		AEstablishment* Establishment = Cast<AEstablishment>(Actor); 
		if (Establishment) 
		{ 
			FVector Location = Establishment->GetActorLocation();
			SaveInstance->ListOfPlacedEstablishments.Add(Location, Establishment->GetClass()); 
			Actor->Destroy();
		}  
	} 

	
	UGenerationLists* GenerationList = Cast<UGenerationLists>(UGameplayStatics::LoadGameFromSlot("@---GENERATION---LIST---@", 0)); 
	
	if (!GenerationList) { GenerationList = Cast<UGenerationLists>(UGameplayStatics::CreateSaveGameObject(UGenerationLists::StaticClass())); }
		
	GenerationList->ListOfSavedGenerations.AddUnique(SaveInstance->GenerationName);
	

	UGameplayStatics::SaveGameToSlot(SaveInstance, SaveInstance->GenerationName.ToString(), 0);
	UGameplayStatics::SaveGameToSlot(GenerationList, "@---GENERATION---LIST---@", 0);

}

void AMainPlayerController::BeginPlay()
{
	GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	bShowMouseCursor = true;
	FString Current = GetWorld()->GetMapName();
	Current.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (Current == "MainMenu")
	{
		ShowMainMenu();
	}
	else
	{
		if (GameInstance->GenerationInfo->GameLoadStatus != ELoadingGameType::New) { SpawnEstatablishments(GameInstance->GenerationInfo->GenerationName); }
		ShowGameHUD();
		if (WBuilder)
		{
			if (!Builder)
			{
				Builder = CreateWidget<UUserWidget>(this, WBuilder);
			}
		}
		FInputModeGameAndUI GameAndUI; 
		SetInputMode(GameAndUI); 
		bGamePaused = false;  
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
			bGamePaused = true;
		}
	}
}

void AMainPlayerController::HidePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		PauseMenu->RemoveFromViewport();
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		bGamePaused = false; 
	}

}

void AMainPlayerController::ShowBuilder_Implementation()
{
	if (Builder)
	{
		Builder->AddToViewport();
		Builder->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::HideBuilder_Implementation()
{

	if (Builder)
	{
		Builder->RemoveFromViewport();
		Builder->SetVisibility(ESlateVisibility::Hidden);

	}
}

void AMainPlayerController::QuitGeneration_Implementation()
{
	SaveGeneration();
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}
