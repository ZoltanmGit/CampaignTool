// Copyright Epic Games, Inc. All Rights Reserved.


#include "CampaignToolGameModeBase.h"
#include "Public/Grid.h"
#include "Public/PlayerCharacter.h"
#include "Public/MapSaveObject.h"
#include "Public/CampaignToolGameInstance.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ACampaignToolGameModeBase::ACampaignToolGameModeBase()
{
	TestFighter.CharacterName = FString("FighterBoy");
	TestFighter.Level = 1;
	TestFighter.ProficiencyBonus = 2;
	TestFighter.Speed = 15.0f; // divided by 5
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
	TestRogue.Speed = 25.0f; // 5 tiles
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
		//InitializeCharacters();
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
	UCampaignToolGameInstance* GameInstance = Cast<UCampaignToolGameInstance>(GetGameInstance());
	if (UMapSaveObject* LoadObject = Cast<UMapSaveObject>(UGameplayStatics::LoadGameFromSlot(GameInstance->SelectedMapSlotName, 0)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Map Loaded from Save"));
		if (Grid)
		{
			FActorSpawnParameters SpawnParams;
			FTransform TransformParams;
			TransformParams.SetLocation(FVector(0.0f, 0.0f, 0.0f));
			Gridptr = GetWorld()->SpawnActor<AGrid>(Grid, TransformParams, SpawnParams);
			TArray<int32> MirroredTileMatrix = LoadObject->TileMatrix;

			//Mirror the matrix on the X axis because of the UI inconsistency
			for (int32 i = 0; i < (LoadObject->Rows * LoadObject->Columns) / 2; i++)
			{
				if (MirroredTileMatrix[i] != MirroredTileMatrix[(LoadObject->Rows * LoadObject->Columns) - (LoadObject->Columns - i % LoadObject->Columns) - ((i / LoadObject->Columns) * LoadObject->Columns)])
				{
					int32 TempIntForSwap = MirroredTileMatrix[i];
					MirroredTileMatrix[i] = MirroredTileMatrix[(LoadObject->Rows * LoadObject->Columns) - (LoadObject->Columns - i % LoadObject->Columns) - ((i / LoadObject->Columns) * LoadObject->Columns)];
					MirroredTileMatrix[(LoadObject->Rows * LoadObject->Columns) - (LoadObject->Columns - i % LoadObject->Columns) - ((i / LoadObject->Columns) * LoadObject->Columns)] = TempIntForSwap;
				}
			}

			if (Gridptr)
			{
				Gridptr->InitializeGrid(LoadObject->Rows, LoadObject->Columns, MirroredTileMatrix);
				UE_LOG(LogTemp, Warning, TEXT("Grid initialized..."));
			}
		}
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
