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

// Function to show the main menu.
void AMainPlayerController::ShowMainMenu_Implementation()
{
	if (WMainMenu)			// If the widget class is not set to 'None, the following block of code is executed.
	{
		if (!MainMenu)		// If the widget instance being referenced does not exist, the widget instance is created with the widget class.
		{
			MainMenu = CreateWidget<UUserWidget>(this, WMainMenu);
		}
		if (MainMenu)		// If the widget instance being referenced does exist, it is added to the viewport and set to visible. 
		{
			MainMenu->AddToViewport();
			MainMenu->SetVisibility(ESlateVisibility::Visible);

			// Sets the input mode to only respond to UI components when the main manu is loaded
			FInputModeUIOnly UIOnly;
			SetInputMode(UIOnly);
		}
	}
}

// Function to hide the main menu.
void AMainPlayerController::HideMainMenu_Implementation()
{

	if (MainMenu)		// If the widget instance being referenced does exist, it is removed from the viewport and set to invisible.
	{
		MainMenu->RemoveFromViewport(); 
		MainMenu->SetVisibility(ESlateVisibility::Hidden); 
	}
}

// Function to show the 'Select Generation' page.
void AMainPlayerController::ShowSelectGeneration_Implementation()
{
	if (WSelectGeneration)			// If the widget class is not set to 'None, the following block of code is executed.
	{
		
		if (!SelectGeneration)		// If the widget instance being referenced does not exist, the widget instance is created with the widget class.
		{
			SelectGeneration = CreateWidget<UUserWidget>(this, WSelectGeneration);
		}
		if (SelectGeneration)		// If the widget instance being referenced does exist, it is added to the viewport and set to visible. 
		{
			SelectGeneration->AddToViewport();
			SelectGeneration->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

// Function to hide the 'Select Generation' page.
void AMainPlayerController::HideSelectGeneration_Implementation()
{

	if (SelectGeneration)		// If the widget instance being referenced does exist, it is removed from the viewport and set to invisible.
	{
		SelectGeneration->RemoveFromViewport();
		SelectGeneration->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Function to show the 'New Generation' page.
void AMainPlayerController::ShowNewGeneration_Implementation()
{
	if (WNewGeneration)			// If the widget class is not set to 'None, the following block of code is executed.
	{
		if (!NewGeneration)		// If the widget instance being referenced does not exist, the widget instance is created with the widget class.
		{
			NewGeneration = CreateWidget<UUserWidget>(this, WNewGeneration);
		}
		if (NewGeneration)		// If the widget instance being referenced does exist, it is added to the viewport and set to visible. 
		{
			NewGeneration->AddToViewport();
			NewGeneration->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

// Function to hide the 'New Generation' page.
void AMainPlayerController::HideNewGeneration_Implementation()
{

	if (NewGeneration)		// If the widget instance being referenced does exist, it is removed from the viewport and set to invisible.
	{
		NewGeneration->RemoveFromViewport();
		NewGeneration->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Function to show the 'Settings' page.
void AMainPlayerController::ShowSettings_Implementation()
{
	if (WSettings)			// If the widget class is not set to 'None, the following block of code is executed.
	{
		if (!Settings)		// If the widget instance being referenced does not exist, the widget instance is created with the widget class.
		{
			Settings = CreateWidget<UUserWidget>(this, WSettings);
		}
		if (Settings)		// If the widget instance being referenced does exist, it is added to the viewport and set to visible. 
		{
			Settings->AddToViewport();
			Settings->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

// Function to hide the 'Settings' page.
void AMainPlayerController::HideSettings_Implementation()
{

	if (Settings)		// If the widget instance being referenced does exist, it is removed from the viewport and set to invisible.
	{
		Settings->RemoveFromViewport();
		Settings->SetVisibility(ESlateVisibility::Hidden);

	}
}

// Function that changes the position and rotation values of the generation data asset. 
void AMainPlayerController::ChangeGenerationInfo(FVector Position, FRotator Rotation, float Length, float CO2, int32 Population, int32 Economy, int32 Month, float TimeElapsed, FName GenerationName, float SustainabilityScore, int32 ResidentialArea) 
{
	GameInstance->GenerationInfo->GenerationName = GenerationName;		// Sets the generation data asset's generation name to the GenerationName parameter. 

	// Sets the in-generation information (CO2, Economy, Population, Residential Area, Sustainability Score) stored in the generation data asset to the corresponding function parameters. 
	GameInstance->GenerationInfo->CO2 = CO2; 
	GameInstance->GenerationInfo->Economy = Economy; 
	GameInstance->GenerationInfo->Population = Population; 
	GameInstance->GenerationInfo->ResidentialArea = ResidentialArea;
	GameInstance->GenerationInfo->SustainabilityScore = SustainabilityScore;
	GameInstance->GenerationInfo->Month = Month;
	GameInstance->GenerationInfo->TimeElapsed = TimeElapsed; 
	
	// Sets player physical information (on the generation) stored in the generation data asset to the corresponding function parameters.
	GameInstance->GenerationInfo->Position = Position; 
	GameInstance->GenerationInfo->Rotation = Rotation;
	GameInstance->GenerationInfo->Length = Length; 
}

// Function that spawns all establishments from a saved slot of a generation.
void AMainPlayerController::SpawnEstatablishments(FName GenerationName)
{
	USaveGeneration* SaveInstance = Cast<USaveGeneration>(UGameplayStatics::LoadGameFromSlot(GenerationName.ToString(), 0));			// Gets a reference to a save instance that has the same name as the generation name provided as a function parameter.


	if (SaveInstance->ListOfPlacedEstablishments.Num() != 0)		// If the list of placed establoshments stored in the save instance is not empty, the following code block executes. 
	{
		FActorSpawnParameters SpawnInfo;		// SpawnInfo declared for spawning actors on the world. 
		// For every 'TPair' element, where a unique FVector coordinate (key) is mapped to an establishment class or child class (value), iterated through the list of placed establishments, an instance of that class is spawned at the FVector posiiton with 'SpawnInfo'.
		for (TPair<FVector, TSubclassOf<AEstablishment>>& Establishment : SaveInstance->ListOfPlacedEstablishments)			
		{
			GetWorld()->SpawnActor<AEstablishment>(Establishment.Value, Establishment.Key, FRotator::ZeroRotator, SpawnInfo);
		}
	} 
}

// Function that updates the sustainability score of the generation based on all establishment instances placed. 
void AMainPlayerController::UpdateSustainability()
{
	float TotalSustainability = 0.f;		// Sets total sustainability to 0.

	// Gets a list of all establishments in the current generation. But list data tyoe is set to 'AActor' (base actor class).
	TArray<AActor*> Establishments = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEstablishment::StaticClass(), Establishments);

	if (Establishments.Num() != 0)		// If the list of actors, that are all establishments, is not empty, the following block of code is executed. 
	{
		// For every actor element in the list, the following is executed. 
		for (AActor* Actor : Establishments) 
		{
			AEstablishment* Establishment = Cast<AEstablishment>(Actor);  // An 'AEstablishment' pointer has a reference to the element, which is casted to the 'AEstablishment' class, 
			// If the pointer is valid, its 'Sustainability' property is added to the total sustainability.
			if (Establishment) 
			{
				TotalSustainability += Establishment->Sustainability;	
			}
		}

		GameInstance->GenerationInfo->SustainabilityScore = TotalSustainability / Establishments.Num();			// The generation's sustainability score is set the total sustainability divided by the number of elements in the list. 
	}
	// Otherwise, if the list of actors is empty, the generation's sustainability score is set to 10.
	else { GameInstance->GenerationInfo->SustainabilityScore = 10.f; }
}

// Function that adds more wealth to the generation's economy value based on the number of commercial establishments placed
void AMainPlayerController::GenerateWealth()
{	
	int32 Counter = 0;		// Sets the counter for commerical establishments to 0.

	// Gets a list of all commercial establishments in the current generation. But list data tyoe is set to 'AActor' (base actor class).
	TArray<AActor*> CommercialEstablishments = TArray<AActor*>(); 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACommercialEstablishment::StaticClass(), CommercialEstablishments);

	// For every actor element in the list, the following is executed. 
	for (AActor* Actor : CommercialEstablishments)
	{
		ACommercialEstablishment* CommercialEstablishment = Cast<ACommercialEstablishment>(Actor);	// An 'ACommercialEstablishment' pointer has a reference to the element, which is casted to the 'ACommericalEstablishment' class,
		// If the pointer is valid, the counter increments by 1.
		if (CommercialEstablishment)
		{
			Counter += 1;
		}
	}

	GameInstance->GenerationInfo->Economy += Counter * int32(GameInstance->GenerationInfo->SustainabilityScore * pow(2.f, (FString::FromInt(GameInstance->GenerationInfo->Population)).Len()));		// The generation's economy is increased by this calculation performed on the counter. 
}

// Updates to CO2 value in the generation. 
void AMainPlayerController::UpdateCO2()
{
	GameInstance->GenerationInfo->CO2 += int32((GameInstance->GenerationInfo->Population / GameInstance->GenerationInfo->SustainabilityScore) * 1500) / 100;   // The generation's CO2 emissions is increased this the calculation performed on the generation's population and sustainability score. 
}

// Function that updates the population in the generation.
void AMainPlayerController::UpdatePopulation()
{
	int32 ProjectedPopulation = 0;		// Projected population intially set to zero.

	// If the sustainability score in the generation score is 5 or more, the projected population is set to the following calculation for benefitting the player and increasing the population, which involves the generation's sustainability score and total residential area. 
	if (GameInstance->GenerationInfo->SustainabilityScore >= 5)		
	{
		ProjectedPopulation = int32((GameInstance->GenerationInfo->ResidentialArea / 20) * (GameInstance->GenerationInfo->SustainabilityScore / 3));
	}
	// Otherwise, if the sustainability in the generation score is less than 5, the projected population is set to a different calculation for giving the player consequences and decreasing the population, which involves the generation's sustainability score and total residential area. 
	else
	{
		ProjectedPopulation = int32((GameInstance->GenerationInfo->ResidentialArea / 20) / (5 - GameInstance->GenerationInfo->SustainabilityScore));
	}

	if (GameInstance->GenerationInfo->Population != ProjectedPopulation) { GameInstance->GenerationInfo->Population = ProjectedPopulation; }	// If the current population in the generation is not equal to the projected population, meaning the player has made changes to the total residential area and/or sustainability score in the generation, the in-generation population is set to the projected population.
}

// Function that gives the user special rewards. This function only executes for every 12 minutes elapsed in the generation. 
void AMainPlayerController::YearlyReward()
{
	GameInstance->GenerationInfo->Economy += int32((GameInstance->GenerationInfo->Population / (FString::FromInt(GameInstance->GenerationInfo->Economy)).Len()) * GameInstance->GenerationInfo->SustainabilityScore * 10);	// Makes an increase to the economy with a calculation involving the generation's current population and economy values. 
}

// Function that updates the generation month value and gives the players rewards based on their changes to the generation.
void AMainPlayerController::MonthUpdate()
{
	// Increments the month value in the generation by 1, and updating the generation's population, CO2 emissions and economy.
	GameInstance->GenerationInfo->Month += 1;
	UpdatePopulation();
	UpdateCO2();
	GenerateWealth();
	// If 12 minutues, out of the total elapsed time, have passed, the player is benefitted wiith the yearly reward. 
	if (GameInstance->GenerationInfo->Month % 12 == 0 && GameInstance->GenerationInfo->Month != 0) 
	{ 
		YearlyReward(); 
	}
}

// Function for creating a generation or loading a generation from a save slot.
void AMainPlayerController::CreateGeneration_Implementation(FName GenerationName) 
{
	// If the player creates a new generation, the generation data asset is changed to default values along with the generation name given by the user.
	if (GameInstance->GenerationInfo->GameLoadStatus == ELoadingGameType::New)
	{
		ChangeGenerationInfo(FVector(0.f, 0.f, 300.f), FRotator(0.f, 0.f, 0.f), 3000.f, 400.f, 0, 10000000, 0, 0.f, GenerationName, 10.f, 0.f);
		LoadInstance = nullptr;
	}
	// If the player continues the generation that was last played, the save instance slot of last generation played is fetched and its stored information is translated to the data in the generation data asset.
	else if (GameInstance->GenerationInfo->GameLoadStatus == ELoadingGameType::Continue)
	{
		UGenerationLists* GenerationList = Cast<UGenerationLists>(UGameplayStatics::LoadGameFromSlot("@---GENERATION---LIST---@", 0));
		LoadInstance = Cast<USaveGeneration>(UGameplayStatics::LoadGameFromSlot(GenerationList->ListOfSavedGenerations.Last().ToString(), 0));
		ChangeGenerationInfo(LoadInstance->Position, LoadInstance->Rotation, LoadInstance->Length, LoadInstance->CO2, LoadInstance->Population, LoadInstance->Economy, LoadInstance->Month, LoadInstance->TimeElapsed, LoadInstance->GenerationName, LoadInstance->SustainabilityScore, LoadInstance->ResidentialArea);
	}
	// If the player selects a saved instance of a previous generation, the save instance slot of the selected generation is fetched and its stored information is translated to the data in the generation data asset. 
	else if (GameInstance->GenerationInfo->GameLoadStatus == ELoadingGameType::Load)
	{
		UGenerationLists* GenerationList = Cast<UGenerationLists>(UGameplayStatics::LoadGameFromSlot("@---GENERATION---LIST---@", 0));
		LoadInstance = Cast<USaveGeneration>(UGameplayStatics::LoadGameFromSlot(GenerationName.ToString(), 0));
		ChangeGenerationInfo(LoadInstance->Position, LoadInstance->Rotation, LoadInstance->Length, LoadInstance->CO2, LoadInstance->Population, LoadInstance->Economy, LoadInstance->Month, LoadInstance->TimeElapsed, LoadInstance->GenerationName, LoadInstance->SustainabilityScore, LoadInstance->ResidentialArea);
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("City"));		// The game loads the city map in a new level.
}

// Functions to show the HUD overlay whenever loading or creating a generation.
void AMainPlayerController::ShowGameHUD_Implementation()
{
	if (WGameHUD)			// If the widget class is not set to 'None, the following block of code is executed.
	{
		if (!GameHUD)		// If the widget instance being referenced does not exist, the widget instance is created with the widget class.
		{
			GameHUD = CreateWidget<UUserWidget>(this, WGameHUD);
		}
		if (GameHUD)		// If the widget instance being referenced does exist, it is added to the viewport and set to visible. 
		{
			GameHUD->AddToViewport();
			GameHUD->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

// Functions to hide the HUD overlay whenever loading or creating a generation.
void AMainPlayerController::HideGameHUD_Implementation() 
{
	if (GameHUD)		// If the widget instance being referenced does exist, it is removed from the viewport and set to invisible.
	{
		GameHUD->RemoveFromViewport();
		GameHUD->SetVisibility(ESlateVisibility::Hidden);

	}
}

// Function that changes the position and rotation values of the generation data asset. 
void AMainPlayerController::TranslatePlayerInformation(FVector& Position, FRotator& Rotation)
{
	GameInstance->GenerationInfo->Position = Position;
	GameInstance->GenerationInfo->Rotation = Rotation;
}

// Function that saves generation data asset information to a new or exisiting save instance. 
void AMainPlayerController::SaveGeneration()
{
	USaveGeneration* SaveInstance = Cast<USaveGeneration>(UGameplayStatics::CreateSaveGameObject(USaveGeneration::StaticClass()));			// Creates a pointer reference to a created save game object that is casted to the 'USaveGeneration' class.
	// Player physical information scored in the generation data asset is translated to the save instance.
	SaveInstance->Position = GameInstance->GenerationInfo->Position;
	SaveInstance->Rotation = GameInstance->GenerationInfo->Rotation;
	SaveInstance->Length = GameInstance->GenerationInfo->Length;
	
	// In-generation values stored in the generation data asset is translated to the save instance.  
	SaveInstance->Population = GameInstance->GenerationInfo->Population; 
	SaveInstance->ResidentialArea = GameInstance->GenerationInfo->ResidentialArea;
	SaveInstance->CO2 = GameInstance->GenerationInfo->CO2; 
	SaveInstance->Economy = GameInstance->GenerationInfo->Economy;
	SaveInstance->SustainabilityScore = GameInstance->GenerationInfo->SustainabilityScore;
	SaveInstance->Month = GameInstance->GenerationInfo->Month;
	SaveInstance->TimeElapsed = GameInstance->GenerationInfo->TimeElapsed;
	
	// Name of the generation, which is kept in the generation data asset, is translated to the save instance. 
	SaveInstance->GenerationName = GameInstance->GenerationInfo->GenerationName; 

	// Gets a list of all establishments in the current generation. But list data tyoe is set to 'AActor' (base actor class).
	TArray<AActor*> Establishments = TArray<AActor*>(); 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEstablishment::StaticClass(), Establishments); 

	// For every actor element iterated through the list, the following is executed. 
	for (AActor* &Actor : Establishments) 
	{ 
		AEstablishment* Establishment = Cast<AEstablishment>(Actor);	// Actor eleement is casted to 'AEstablishment' class.
		if (Establishment) 
		{ 
			FVector Location = Establishment->GetActorLocation();		// Location of the establishment instance is fetched.
			SaveInstance->ListOfPlacedEstablishments.Add(Location, Establishment->GetClass());		// The establishment instance's location and establishment instance's class is respectively stored as a mapped key-value pair element, that is added to the 'TMap' container that handles all saved establishments in the save instance. 

			Actor->Destroy();		// Establishment on the generation is destroyed or deconstructed.
		}  
	} 

	
	UGenerationLists* GenerationList = Cast<UGenerationLists>(UGameplayStatics::LoadGameFromSlot("@---GENERATION---LIST---@", 0));		// Makes a reference to a loaded slot called '@---GENERATION---LIST---@', which stores a list of names of all saved generations. The loaded slot is casted to its 'UGenerationLists' class.
	
	if (!GenerationList) { GenerationList = Cast<UGenerationLists>(UGameplayStatics::CreateSaveGameObject(UGenerationLists::StaticClass())); }		// If the loaded slot '@---GENERATION---LIST---@' does not exist, it gets created as a new save game object, which is also casted to the 'UGenerationLists' class.
	
	if (GenerationList->ListOfSavedGenerations.Contains(SaveInstance->GenerationName)) { GenerationList->ListOfSavedGenerations.Remove(SaveInstance->GenerationName); }		// If the name of the generation save instance exists in the list of saved generation stored in '@---GENERATION---LIST---@', it gets removed from the list.
	
	GenerationList->ListOfSavedGenerations.AddUnique(SaveInstance->GenerationName);		// The name of generation save instance is added to the list of saved generation stored in '@---GENERATION---LIST---@'. In the case of the name existing in the list and getting removed, it gets appended to the end of the list, in order to ensure that the generation that the name refers to is loaded if the player selects the 'Continue Game' option in the main menu,
	
	// The generation save instance and '@---GENERATION---LIST---@' is saved to a new or existing slot with the same name. 
	UGameplayStatics::SaveGameToSlot(SaveInstance, SaveInstance->GenerationName.ToString(), 0);
	UGameplayStatics::SaveGameToSlot(GenerationList, "@---GENERATION---LIST---@", 0);

}

// Called when the game starts or when spawned. 
// This function resets when the game opens a new map in a new level.
void AMainPlayerController::BeginPlay()
{
	GameInstance = Cast<UMainGameInstance>(GetGameInstance());			// Gets 'GameInstance' to point to the game instance being used in the game and casts it to 'UMainGameInstance' class to access its generation data asset pointer. 
	// Gets the name of the current map. 
	bShowMouseCursor = true;
	FString Current = GetWorld()->GetMapName();
	Current.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	// If the map is the main menu map, the main menu is shown.
	if (Current == "MainMenu")
	{
		ShowMainMenu();
	}
	else						// If the map is not the main menu map, but rather the city map, the following code executes.
	{
		if (GameInstance->GenerationInfo->GameLoadStatus != ELoadingGameType::New) { SpawnEstatablishments(GameInstance->GenerationInfo->GenerationName); }		// If the user isn't creating a new generation, the list of establishments on the saved instance, that is being loaded, are spawned on the generation.
		ShowGameHUD();			// Loads the generation overlay HUD.
		if (WBuilder)			// If the widget class is not set to 'None, the following block of code is executed.
		{
			if (!Builder)		// If the widget instance being referenced does not exist, the widget instance is created with the widget class.
			{
				Builder = CreateWidget<UUserWidget>(this, WBuilder);
			}
		}
		// Sets the input mode to respond with both the game and UI elements. 
		FInputModeGameAndUI GameAndUI; 
		SetInputMode(GameAndUI); 

		bGamePaused = false;	// Sets the game pause state to false. 
	}
}

// Function to show the pause menu.
void AMainPlayerController::ShowPauseMenu_Implementation()
{
	if (WPauseMenu)			// If the widget class is not set to 'None, the following block of code is executed.
	{
		if (!PauseMenu)		// If the widget instance being referenced does not exist, the widget instance is created with the widget class.
		{
			PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		}
		if (PauseMenu)		// If the widget instance being referenced does exist, it is added to the viewport and set to visible. 
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Visible);
			bGamePaused = true;		// Game paused boolean value is set to true. 
		}
	}
}

// Function to hide the pause menu.
void AMainPlayerController::HidePauseMenu_Implementation()
{
	if (PauseMenu)		// If the widget instance being referenced does exist, it is removed from the viewport and set to invisible.
	{
		PauseMenu->RemoveFromViewport();
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		bGamePaused = false;		// Game paused boolean value is set to false.
	}

}

// Function to show the builder.
void AMainPlayerController::ShowBuilder_Implementation()
{
	if (Builder)		// If the widget instance being referenced does exist, it is added to the viewport and set to visible. 
	{
		Builder->AddToViewport();
		Builder->SetVisibility(ESlateVisibility::Visible);
	}
}

// Function to hide the builder.
void AMainPlayerController::HideBuilder_Implementation()
{

	if (Builder)		// If the widget instance being referenced does exist, it is removed from the viewport and set to invisible.
	{
		Builder->RemoveFromViewport();
		Builder->SetVisibility(ESlateVisibility::Hidden);

	}
}

// Function for quitting a generation.
void AMainPlayerController::QuitGeneration_Implementation()
{
	SaveGeneration();		// Saves the generation information to an exisiting or new save instance
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));		// Opens the main manu map in a new level.
}
