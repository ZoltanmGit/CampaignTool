// Copyright Epic Games, Inc. All Rights Reserved.


#include "CampaignToolGameModeBase.h"
#include "GridSystem/Grid.h"
#include "Utilities/IndicatorActor.h"
#include "Utilities/DiceRoller.h"
#include "AbilitySystem/AbilityStorage.h"
#include "GameFramework/SpringArmComponent.h"
#include "InventorySystem/ItemStorage.h"
#include "Character/PlayerCharacter.h"
#include "Character/CharacterSaveObject.h"
#include "Character/EnemyController.h"
#include "Character/AiCharacter.h"
#include "Character/AttributesComponent.h"
#include "Persistence/MapSaveObject.h"
#include "Public/CampaignToolGameInstance.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ACampaignToolGameModeBase::ACampaignToolGameModeBase()
{
	EnemyController = CreateDefaultSubobject<AEnemyController>(TEXT("Enemy Controller"));

	turnIndex = -1;
	preparationTurnIndex = -1;
	bIsGameOver = false;
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
		InitializeCharacters(); 

		/** Enemy Init **/
		InitializeEnemies();

		/** Sort the Characters based on initiative **/
		SortInitiative();

		/** Start the first Turn **/
		NextTurn();

		Super::BeginPlay();

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode - BeginPlay(): UserController is nullptr"));
	}
}

void ACampaignToolGameModeBase::NextTurn()
{
	if (!bIsGameOver)
	{
		if (preparationTurnIndex < PlayerCharacters.Num() - 1)
		{
			preparationTurnIndex++;

			if (PlayerCharacters[preparationTurnIndex]->bIsPlayerCharacter)
			{
				UserController->UnPossess();
				APlayerCharacter* character = Cast<APlayerCharacter>(PlayerCharacters[preparationTurnIndex]);

				/** If the previous character was a player character **/
				if (previousCharacter != nullptr)
				{
					/** Set the Rotation of the new possessed pawn to the previous one's **/
					character->CharacterSpringArm->SetWorldRotation(previousCharacter->CharacterSpringArm->GetComponentRotation());
					previousCharacter->CharacterSpringArm->SetWorldLocation(previousCharacter->GetActorLocation());
				}

				character->DefaultController = UserController;
				UserController->Possess(PlayerCharacters[preparationTurnIndex]);
				previousCharacter = Cast<APlayerCharacter>(PlayerCharacters[preparationTurnIndex]);
			}

			Cast<APlayerCharacter>(PlayerCharacters[preparationTurnIndex])->BeginPreparationTurn();
		}
		else
		{
			if (turnIndex == Characters.Num() - 1)
			{
				turnIndex = 0;
			}
			else
			{
				turnIndex++;
			}

			if (Characters[turnIndex]->bIsAlive)
			{
				if (Characters[turnIndex]->bIsPlayerCharacter)
				{
					//UserController->UnPossess();
					APlayerCharacter* character = Cast<APlayerCharacter>(Characters[turnIndex]);

					/** If the previous character was a player character **/
					if (previousCharacter != nullptr)
					{
						/** Set the Rotation of the new possessed pawn to the previous one's **/
						character->CharacterSpringArm->SetWorldRotation(previousCharacter->CharacterSpringArm->GetComponentRotation());
						previousCharacter->CharacterSpringArm->SetWorldLocation(previousCharacter->GetActorLocation());
					}
					if (previousCharacter != character)
					{
						UserController->UnPossess();
						character->DefaultController = UserController;
						UserController->Possess(Characters[turnIndex]);
						previousCharacter = Cast<APlayerCharacter>(Characters[turnIndex]);
					}
				}
				Characters[turnIndex]->BeginTurn();
			}
			else
			{
				NextTurn();
			}
		}
	}
}

void ACampaignToolGameModeBase::HandleCharacterDeath(ABaseCharacter* characterThatDied)
{
	UE_LOG(LogTemp, Warning, TEXT("GameMode - HandleEnemyDeathCalled()"));

	int32 Index;
	FTileProperties tile = Gridptr->GetTilePropertiesFromTransform(characterThatDied->GetActorTransform(), Index);
	Gridptr->GridDataArray[Index].bIsOccupied = false;
	Gridptr->GridDataArray[Index].ActorOnTile = nullptr;
	characterThatDied->SetActorHiddenInGame(true);

	if (!CharacterAlive())
	{
		APlayerCharacter* character = Cast<APlayerCharacter>(PlayerCharacters[0]);
		UserController->UnPossess();
		character->DefaultController = UserController;
		UserController->Possess(character);

		character->OnDefeat();
	}
}

void ACampaignToolGameModeBase::HandleEnemyDeath(ABaseCharacter* enemyThatDied)
{
	UE_LOG(LogTemp, Warning, TEXT("GameMode - HandleEnemyDeathCalled()"));
	
	int32 Index;
	FTileProperties tile = Gridptr->GetTilePropertiesFromTransform(enemyThatDied->GetActorTransform(), Index);
	Gridptr->GridDataArray[Index].bIsOccupied = false;
	Gridptr->GridDataArray[Index].ActorOnTile = nullptr;
	enemyThatDied->SetActorHiddenInGame(true);
	
	if (!AnyEnemyAlive())
	{
		APlayerCharacter* character = Cast<APlayerCharacter>(PlayerCharacters[0]);
		UserController->UnPossess();
		character->DefaultController = UserController;
		UserController->Possess(character);
		
		character->OnVictory();
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
	UserController->Possess(PlayerCharacters[0]);
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
			newCharacter->bIsPlayerCharacter = false;
			newCharacter->InitializeEnemyCharacter(Gridptr, Indicatorptr, AbilityStorageptr, ItemStorageptr);
			newCharacter->Initiative = newCharacter->DiceRoller->Roll(20) + newCharacter->CharacterAttributes->GetModifier(EAbilityType::Dexterity);
		}
		
		Characters.Add(newCharacter);
		EnemyCharacters.Add(newCharacter);
	}
}

void ACampaignToolGameModeBase::SortInitiative()
{
	for (int32 i = 0; i < Characters.Num() - 1; i++)
	{
		for (int32 j = 0; j < Characters.Num() - 1 - i; j++)
		{
			if (Characters[j]->Initiative < Characters[j + 1]->Initiative)
			{
				ABaseCharacter* bucketCharacter = Characters[j];
				Characters[j] = Characters[j + 1];
				Characters[j + 1] = bucketCharacter;
			}
		}
	}

	for (int32 i = 0; i < Characters.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Initiative: %i"),*Characters[i]->CharacterAttributes->Stats.CharacterName, Characters[i]->Initiative);
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
	if (character.Class == EClass::Fighter)
	{
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(FighterClass, TransformParams, SpawnParams);
	}
	else if (character.Class == EClass::Rogue)
	{
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RogueClass, TransformParams, SpawnParams);
	}
	else if (character.Class == EClass::Wizard)
	{
		newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(WizardClass, TransformParams, SpawnParams);
	}
	// Initialize Character
	if (newCharacter != nullptr && Gridptr != nullptr && Indicatorptr != nullptr && AbilityStorageptr != nullptr && ItemStorageptr != nullptr)
	{
		Gridptr->GridDataArray[(x * Gridptr->Columns) + y].ActorOnTile = newCharacter;
		newCharacter->bIsPlayerCharacter = true;
		newCharacter->InitializeCharacter(character, Gridptr, Indicatorptr, AbilityStorageptr, ItemStorageptr);
		newCharacter->Initiative = newCharacter->DiceRoller->Roll(20) + newCharacter->CharacterAttributes->GetModifier(EAbilityType::Dexterity);
		
	}
	
	APlayerCharacter* newPlayerCharacter = Cast<APlayerCharacter>(newCharacter);
	newPlayerCharacter->CharacterSpringArm->SetWorldLocation(TransformParams.GetLocation());
	newPlayerCharacter->OnInventoryChange();
	

	Characters.Add(newCharacter);
	PlayerCharacters.Add(newCharacter);
}

bool ACampaignToolGameModeBase::AnyEnemyAlive()
{
	for (int32 i = 0; i < EnemyCharacters.Num(); i++)
	{
		if (EnemyCharacters[i]->bIsAlive == true)
		{
			
			return true;
		}
	}
	bIsGameOver = true;
	return false;
}

bool ACampaignToolGameModeBase::CharacterAlive()
{
	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (PlayerCharacters[i]->bIsAlive)
		{
			return true;
		}
	}
	bIsGameOver = true;
	return false;
}
