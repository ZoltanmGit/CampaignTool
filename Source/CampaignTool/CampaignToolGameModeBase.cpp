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

	MapChoice = 4;
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
		//InitializeMovementController();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not found!"));
	}
}

void ACampaignToolGameModeBase::InitializeGrid()
{
	// 10 x 10
	int tempMatrix[100] = { 0,0,0,0,1,1,0,0,0,0,
							0,0,4,4,1,1,4,4,0,0,
							0,0,4,3,3,3,3,4,0,0,
							0,0,4,3,3,3,3,4,0,0,
							0,0,4,3,3,3,3,4,0,0,
							0,0,4,4,1,1,4,4,0,0,
							-1,-1,-1,-1,1,1,-1,-1,-1,-1,
							5,5,5,5,1,1,5,5,5,5,
							1,1,1,1,1,1,1,1,1,1,
							2,2,2,2,2,2,2,2,2,2
	};
	//15 x 15
	int tempMatrix2[225] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
							1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,
							2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,
							1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,
							0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,
							1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,
							0,0,0,1,0,1,1,0,1,1,1,0,1,1,0,
							0,0,0,1,0,0,0,0,0,1,1,0,1,0,0,
							0,0,0,1,1,1,1,0,0,1,1,0,1,1,1,
							0,0,0,0,0,0,0,0,0,1,1,0,0,0,0
	};
	//4 x 13
	int tempMatrix3[52] = { 4,-1, 4, 4, 4,-1, 4, 4, 4,-1, 4, 4, 4,
							4,-1, 4,-1, 4,-1, 4,-1, 4,-1, 4,-1, 4,
							4,-1, 4,-1, 4,-1, 4,-1, 4,-1, 4,-1, 4,
							4, 4, 4,-1, 4, 4, 4,-1, 4, 4, 4,-1, 4
	};
	//10 x 10
	int tempMatrix4[100] = { 5,5,5,5,5,5,5,5,5,5,
							5,5,5,5,5,5,5,5,5,5,
							5,5,5,5,5,5,5,5,5,5,
							5,5,5,5,5,5,5,5,5,5,
							5,5,5,5,5,5,5,5,5,5,
							5,5,5,5,5,5,5,5,5,5,
							5,5,5,5,5,5,5,5,5,5,
							5,5,5,5,5,5,5,5,5,5,
							5,5,5,5,5,5,5,5,5,5,
							5,5,5,5,5,5,5,5,5,5
	};
	if (Grid)
	{
		FActorSpawnParameters SpawnParams;
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(0.0f, 0.0f, 0.0f));
		Gridptr = GetWorld()->SpawnActor<AGrid>(Grid, TransformParams, SpawnParams);

		MapChoice = FMath::Clamp(MapChoice, 1, 4);

		switch (MapChoice)
		{
		case 1:
			Gridptr->InitializeGrid(10, 10, tempMatrix);
			break;
		case 2:
			Gridptr->InitializeGrid(15, 15, tempMatrix2);
			break;
		case 3:
			Gridptr->InitializeGrid(4, 13, tempMatrix3);
			break;
		case 4:
			Gridptr->InitializeGrid(10, 10, tempMatrix4);
			break;
		default:
			break;
		}

		//Gridptr->InitializeGrid(10, 10, tempMatrix4);
		UE_LOG(LogTemp, Warning, TEXT("Grid initialized..."));
	}
}

void ACampaignToolGameModeBase::InitializeCharacters()
{
	SpawnCharacter(TestFighter, 0, 0);
	SpawnCharacter(TestRogue, 1, 0);


	if (Characters[0] != nullptr)
	{
		APlayerCharacter* character = Cast<APlayerCharacter>(Characters[0]);
		character->DefaultController = UserController; 
		UserController->Possess(Characters[0]);
		character->BeginTurn();
	}
}

void ACampaignToolGameModeBase::InitializeMovementController()
{
	if (MovementController)
	{
		FActorSpawnParameters SpawnParams;
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(-100.0f, -100.0f, 0.0f));
		MovementControllerptr = GetWorld()->SpawnActor<AMovementController>(MovementController,TransformParams,SpawnParams);
		MovementControllerptr->Characters = this->Characters;
		UE_LOG(LogTemp, Warning, TEXT("MovementController initialized."));
	}
}

void ACampaignToolGameModeBase::SpawnCharacter(FCharacterStruct character, int32 x, int32 y)
{
	FActorSpawnParameters SpawnParams;
	FTransform TransformParams;
	TransformParams.SetLocation(FVector((x*Gridptr->fieldSize)+(Gridptr->fieldSize/2), (y* Gridptr->fieldSize) + (Gridptr->fieldSize / 2), 50.0f));
	Gridptr->GridDataArray[(x * Gridptr->Columns) + y].bIsOccupied = true;
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
		Gridptr->GridDataArray[(x * Gridptr->Columns) + y].ActorOnTile = newCharacter;
		newCharacter->InitializeCharacter(character,Gridptr);
	}
	Characters.Add(newCharacter);
}
