// Copyright Epic Games, Inc. All Rights Reserved.


#include "CampaignToolGameModeBase.h"
#include "GridSystem/Grid.h"
#include "Utilities/IndicatorActor.h"
#include "AbilitySystem/AbilityStorage.h"
#include "GameFramework/SpringArmComponent.h"
#include "InventorySystem/ItemStorage.h"
#include "Character/PlayerCharacter.h"
#include "Character/CharacterSaveObject.h"
#include "Character/EnemyController.h"
#include "Character/AiCharacter.h"
#include "Persistence/MapSaveObject.h"
#include "Public/CampaignToolGameInstance.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ACampaignToolGameModeBase::ACampaignToolGameModeBase()
{
	EnemyController = CreateDefaultSubobject<AEnemyController>(TEXT("Enemy Controller"));

	turnIndex = -1;
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

		/** AbilityStorage Init **/
		InitializeAbilityStorage();

		/** ItemStorage Init **/
		InitializeItemStorage();

		/** Character Init **/
		InitializeCharacters(); //requires grid and indicator to be initialized beforehand

		/** Enemy Init **/
		InitializeEnemies();

		/** Start the first Turn **/
		NextTurn();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(""));
	}
}

void ACampaignToolGameModeBase::NextTurn()
{
	if (turnIndex == Characters.Num() - 1)
	{
		turnIndex = 0;
	}
	else
	{
		turnIndex++;
	}
	
	if (Characters[turnIndex]->bIsPlayerCharacter)
	{
		UserController->UnPossess();
		APlayerCharacter* character = Cast<APlayerCharacter>(Characters[turnIndex]);

		if (previousCharacter != nullptr)
		{
			/** Set the Rotation of the new possessed pawn to the previous one's **/
			character->CharacterSpringArm->SetWorldRotation(previousCharacter->CharacterSpringArm->GetComponentRotation());
			previousCharacter->CharacterSpringArm->SetWorldLocation(previousCharacter->GetActorLocation());
		}

		character->DefaultController = UserController;
		UserController->Possess(Characters[turnIndex]);
		previousCharacter = Cast<APlayerCharacter>(Characters[turnIndex]);
	}
	Characters[turnIndex]->BeginTurn();
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

void ACampaignToolGameModeBase::InitializeAbilityStorage()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform TransformParams;
	TransformParams.SetLocation(FVector(0.0f, 0.0f, 0.0f));
	AbilityStorageptr = GetWorld()->SpawnActor<AAbilityStorage>(AbilityStorage, TransformParams, SpawnParams);
}

void ACampaignToolGameModeBase::InitializeItemStorage()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform TransformParams;
	TransformParams.SetLocation(FVector(0.0f, 0.0f, 0.0f));
	ItemStorageptr = GetWorld()->SpawnActor<AItemStorage>(ItemStorage, TransformParams, SpawnParams);
}

void ACampaignToolGameModeBase::InitializeCharacters()
{
	UCampaignToolGameInstance* GameInstance = Cast<UCampaignToolGameInstance>(GetGameInstance());
	
	if (GameInstance != nullptr)
	{
		for (auto& Element : GameInstance->IndexForCharacter)
		{
			UCharacterSaveObject* CharacterToLoad = Cast<UCharacterSaveObject>(UGameplayStatics::LoadGameFromSlot(Element.Value, 0));
			if (CharacterToLoad != nullptr)
			{
				int32 row = Element.Key / Gridptr->Columns;
				int32 column = Element.Key % Gridptr->Columns;
				// row needs to be mirrored
				row = (Gridptr->Rows - 1) - row;
				SpawnCharacter(CharacterToLoad->SavedCharacterStruct,row,column);
			}
		}
	}

	/*if (Characters[0] != nullptr)
	{
		APlayerCharacter* character = Cast<APlayerCharacter>(Characters[0]);
		character->DefaultController = UserController; 
		UserController->Possess(Characters[0]);
		character->BeginTurn();
	}*/
}

void ACampaignToolGameModeBase::InitializeEnemies()
{
	UCampaignToolGameInstance* GameInstance = Cast<UCampaignToolGameInstance>(GetGameInstance());
	
	for (auto& Element : GameInstance->EnemySpawnMap)
	{
		int32 row = Element.Key / Gridptr->Columns;
		int32 column = Element.Key % Gridptr->Columns;
		// row needs to be mirrored
		row = (Gridptr->Rows - 1) - row;
		
		FActorSpawnParameters SpawnParams;
		FTransform TransformParams;
		TransformParams.SetLocation(FVector((row * Gridptr->fieldSize) + (Gridptr->fieldSize / 2), (column * Gridptr->fieldSize) + (Gridptr->fieldSize / 2), 50.0f));
		Gridptr->GridDataArray[(row * Gridptr->Columns) + column].bIsOccupied = true;
		
		ABaseCharacter* newCharacter = GetWorld()->SpawnActor<AAiCharacter>(Element.Value, TransformParams, SpawnParams);
		if (newCharacter != nullptr && Gridptr != nullptr && Indicatorptr != nullptr && AbilityStorageptr != nullptr && ItemStorageptr != nullptr)
		{
			Gridptr->GridDataArray[(row * Gridptr->Columns) + column].ActorOnTile = newCharacter;
			newCharacter->InitializeEnemyCharacter(Gridptr, Indicatorptr, AbilityStorageptr, ItemStorageptr);
			newCharacter->bIsPlayerCharacter = false;
		}
		
		Characters.Add(newCharacter);
		EnemyCharacters.Add(newCharacter);
	}
}

void ACampaignToolGameModeBase::SpawnCharacter(FCharacterStruct character, int32 x, int32 y)
{
	FActorSpawnParameters SpawnParams;
	FTransform TransformParams;
	TransformParams.SetLocation(FVector((x*Gridptr->fieldSize)+(Gridptr->fieldSize/2), (y* Gridptr->fieldSize) + (Gridptr->fieldSize / 2), 50.0f));
	Gridptr->GridDataArray[(x * Gridptr->Columns) + y].bIsOccupied = true;
	ABaseCharacter* newCharacter = nullptr;
	
	
	// Spawn Character from template
	// There is no way for a character to gain levels in multiple classes. Therefore this solution is adequate.
	if (character.ClassLevelMap.Find(EClass::Fighter) > 0)
	{
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(FighterClass, TransformParams, SpawnParams);
	}
	else if (character.ClassLevelMap.Find(EClass::Rogue) > 0)
	{
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RogueClass, TransformParams, SpawnParams);
	}
	else if (character.ClassLevelMap.Find(EClass::Wizard) > 0)
	{
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RogueClass, TransformParams, SpawnParams);
	}
	// Initialize Character
	if (newCharacter != nullptr && Gridptr != nullptr && Indicatorptr != nullptr && AbilityStorageptr != nullptr && ItemStorageptr != nullptr)
	{
		Gridptr->GridDataArray[(x * Gridptr->Columns) + y].ActorOnTile = newCharacter;
		newCharacter->InitializeCharacter(character, Gridptr, Indicatorptr, AbilityStorageptr, ItemStorageptr);
		newCharacter->bIsPlayerCharacter = true;
	}
	
	APlayerCharacter* newPlayerCharacter = Cast<APlayerCharacter>(newCharacter);
	newPlayerCharacter->CharacterSpringArm->SetWorldLocation(TransformParams.GetLocation());
	

	Characters.Add(newCharacter);
	PlayerCharacters.Add(newCharacter);
}
