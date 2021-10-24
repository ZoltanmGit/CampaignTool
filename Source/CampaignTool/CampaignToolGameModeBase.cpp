// Copyright Epic Games, Inc. All Rights Reserved.


#include "CampaignToolGameModeBase.h"
#include "Public/Grid.h"
#include "Public/IndicatorActor.h"
#include "Public/PlayerCharacter.h"
#include "Public/MapSaveObject.h"
#include "Public/CampaignToolGameInstance.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ACampaignToolGameModeBase::ACampaignToolGameModeBase()
{
	TestFighter.bIsPlayerCharacter = true;
	TestFighter.ArmorClass = 10;
	TestFighter.CharacterName = "BlackBetty";
	TestFighter.Class = EClass::Fighter;
	TestFighter.Speed = 30.0f;

	TestRogue.bIsPlayerCharacter = true;
	TestRogue.ArmorClass = 10;
	TestRogue.CharacterName = "Bames Jond";
	TestRogue.Class = EClass::Rogue;
	TestRogue.Speed = 25.0f;
}

void ACampaignToolGameModeBase::BeginPlay()
{
	UserController = Cast<APlayerController>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerController::StaticClass()));
	if (UserController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController found"));
		
		/** Grid Init **/
		InitializeGrid();

		/** Indicator Init **/
		InitializeIndicator();

		/** Character Init **/
		InitializeCharacters(); //requires grid and indicator to be initialized beforehand
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

void ACampaignToolGameModeBase::InitializeIndicator()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform TransformParams;
	TransformParams.SetLocation(FVector(0.0f, 0.0f, 0.0f));
	Indicatorptr = GetWorld()->SpawnActor<AIndicatorActor>(Indicator, TransformParams, SpawnParams);
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
	case EClass::Fighter:
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(FighterClass, TransformParams, SpawnParams);
		break;
	case EClass::Rogue:
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RogueClass, TransformParams, SpawnParams);
		break;
	default:
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RogueClass, TransformParams, SpawnParams);
		break;
	}
	if (newCharacter != nullptr)
	{
		Gridptr->GridDataArray[(x * Gridptr->Columns) + y].ActorOnTile = newCharacter;
		newCharacter->InitializeCharacter(character,Gridptr,Indicatorptr);
	}
	Characters.Add(newCharacter);
}
