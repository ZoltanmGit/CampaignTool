// Copyright Epic Games, Inc. All Rights Reserved.


#include "CampaignToolGameModeBase.h"
#include "Public/MovementController.h"
#include "Public/Grid.h"
#include "Public/PlayerCharacter.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ACampaignToolGameModeBase::ACampaignToolGameModeBase()
{
	TestFighter.CharacterName = FString("FighterBoy");
	TestFighter.Level = 1;
	TestFighter.ProficiencyBonus = 2;
	TestFighter.Speed = 25.0f;
	TestFighter.HitDie = 12;
	TestFighter.Strength = 12;
	TestFighter.Dexterity = 14;
	TestFighter.Intelligence = 8;
	TestFighter.Constitution = 18;
	TestFighter.Wisdom = 9;
	TestFighter.Charisma = 8;
	TestFighter.Race = Race::Tiefling;
	TestFighter.Class = Class::Fighter;
	TestFighter.Alignment = Alignment::N;
	TestFighter.ProficiencyArray.Add(Proficiency::Shortswords);
	TestFighter.LanguageArray.Add(Language::Infernal);

	TestRogue.CharacterName = FString("McRogue");
	TestRogue.Level = 1;
	TestRogue.ProficiencyBonus = 2;
	TestRogue.Speed = 30.0f;
	TestRogue.HitDie = 8;
	TestRogue.Strength = 8;
	TestRogue.Dexterity = 18;
	TestRogue.Intelligence = 14;
	TestRogue.Constitution = 8;
	TestRogue.Wisdom = 12;
	TestRogue.Charisma = 14;
	TestRogue.Race = Race::Dwarf;
	TestRogue.Class = Class::Rogue;
	TestRogue.Alignment = Alignment::LE;
	TestRogue.ProficiencyArray.Add(Proficiency::Daggers);
	TestRogue.ProficiencyArray.Add(Proficiency::ThievesTools);
	TestRogue.LanguageArray.Add(Language::Common);
	TestRogue.LanguageArray.Add(Language::Dwarvish);
}

void ACampaignToolGameModeBase::BeginPlay()
{
	UserController = Cast<APlayerController>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerController::StaticClass()));
	if (UserController)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("PlayerController found"));

		//Spawn Grid
		InitializeGrid();
		//Spawn Characters
		InitializeCharacters();
		//Spawn MovementController
		InitializeMovementController();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not found!"));
	}
}

void ACampaignToolGameModeBase::InitializeGrid()
{
	if (Grid)
	{
		FActorSpawnParameters SpawnParams;
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(0.0f, 0.0f, 0.0f));
		Gridptr = GetWorld()->SpawnActor<AGrid>(Grid, TransformParams, SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("Grid initialized."));
	}
}

void ACampaignToolGameModeBase::InitializeCharacters()
{
	SpawnCharacter(TestFighter, 0, 0);
	SpawnCharacter(TestRogue, 2, 2);
	SpawnCharacter(TestFighter, 2, 5);
	SpawnCharacter(TestFighter, 3, 5);
	SpawnCharacter(TestFighter, 4, 5);


	if (Characters[0] != nullptr)
	{
		APlayerCharacter* character = Cast<APlayerCharacter>(Characters[0]);
		character->DefaultController = UserController; 
		UserController->Possess(Characters[0]);
		character->bIsPossessed = true;
		character->BeginTurn();
	}
	/*FActorSpawnParameters SpawnParams;
	if (FighterClass)
	{
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(50.0f, 50.0f, 50.0f));
		APlayerCharacter* newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(FighterClass, TransformParams, SpawnParams);
		newCharacter->DefaultController = UserController;
		UserController->Possess(newCharacter);
		newCharacter->bIsPossessed = true;
		newCharacter->BeginTurn();
		UE_LOG(LogTemp, Warning, TEXT("Fighter spawned. (And possessed)"));
	}
	if (RogueClass)
	{
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(150.0f, 50.0f, 50.0f));
		APlayerCharacter* newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RogueClass, TransformParams, SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("Rogue spawned."));
	}
	/*if (WizardClass)
	{
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(250.0f, 50.0f, 50.0f));
		APlayerCharacter* newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(WizardClass, TransformParams, SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("Wizard spawned."));
	}*/
}

void ACampaignToolGameModeBase::InitializeMovementController()
{
	if (MovementController)
	{
		FActorSpawnParameters SpawnParams;
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(-100.0f, -100.0f, 0.0f));
		GetWorld()->SpawnActor<AMovementController>(MovementController,TransformParams,SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("MovementController initialized."));
	}
}

void ACampaignToolGameModeBase::SpawnCharacter(FCharacterStruct character, int32 x, int32 y)
{
	FActorSpawnParameters SpawnParams;
	FTransform TransformParams;
	TransformParams.SetLocation(FVector((x*Gridptr->fieldSize)+(Gridptr->fieldSize/2), (y* Gridptr->fieldSize) + (Gridptr->fieldSize / 2), 50.0f));
	Gridptr->GridDataArray[(x * Gridptr->MapSize) + y].bIsOccupied = true;
	ABaseCharacter* newCharacter = nullptr;
	switch (character.Class)
	{
	case Class::Fighter:
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(FighterClass, TransformParams, SpawnParams);
		break;
	case Class::Rogue:
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RogueClass, TransformParams, SpawnParams);
		break;
	default:
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RogueClass, TransformParams, SpawnParams);
		break;
	}
	if (newCharacter != nullptr)
	{
		newCharacter->InitializeCharacter(character);
	}
	Characters.Add(newCharacter);
}
