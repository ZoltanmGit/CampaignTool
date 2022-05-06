// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AiCharacter.h"
#include "Character/AttributesComponent.h"
#include "../../CampaignToolGameModeBase.h"
#include "MovementSystem/PathfinderComponent.h"
#include "MovementSystem/Movercomponent.h"
#include "GridSystem/Grid.h"

void AAiCharacter::BeginTurn()
{
	EnemyFocus = nullptr;
	bAction = true;
	bMovementAction = true;
	bBonusAction = true;
	ResolveMovement();
}

void AAiCharacter::EndTurn()
{
	ACampaignToolGameModeBase* GameMode = Cast<ACampaignToolGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->NextTurn();
}

void AAiCharacter::ResolveMovement()
{

	bCanMove = true;
	bCanAct = true;
	/** If NPC does not have neighbor player character **/
	if (GetNeighbor() == nullptr)
	{
		/** Fill up valid index map **/
		int32 unusedIndex;//Unused
		FTileProperties tile = Grid->GetTilePropertiesFromTransform(this->GetActorTransform(), unusedIndex);
		Pathfinder->GetValidMovementIndexes(tile.Row, tile.Column, 100);

		ACampaignToolGameModeBase* GameMode = Cast<ACampaignToolGameModeBase>(GetWorld()->GetAuthGameMode());


		/** Get Route to first player character**/
		int32 playerIndex;
		FTileProperties playerTile = Grid->GetTilePropertiesFromTransform(GameMode->PlayerCharacters[0]->GetActorTransform(), playerIndex);
		Pathfinder->GetRouteFromIndexes(GameMode->PlayerCharacters[0]->GetRow(), GameMode->PlayerCharacters[0]->GetColumn());
		
		/** Init temporary variables **/
		int32 currentDistanceToFocusedCharacter = -1;
		int32 comparisonDistance = -1;

		/** If there is a route to the first character **/
		if (Pathfinder->Route.Num() > 2)
		{
			currentDistanceToFocusedCharacter = 0;
			// then the first character will be the movement focus
			EnemyFocus = GameMode->PlayerCharacters[0];
			
			/** Get the route's 'weight' **/
			for (int32 i = 1; i < Pathfinder->Route.Num()-1; i++)
			{
				currentDistanceToFocusedCharacter += *Pathfinder->ValidIndexMap.Find(Pathfinder->Route[i]);
			}
		}

		// Iterate through remaining player characters
		for (int32 i = 1; i < GameMode->PlayerCharacters.Num(); i++)
		{
			/** Get Route to playercharacter**/
			playerIndex;
			playerTile = Grid->GetTilePropertiesFromTransform(GameMode->PlayerCharacters[i]->GetActorTransform(), playerIndex);
			Pathfinder->GetRouteFromIndexes(GameMode->PlayerCharacters[i]->GetRow(), GameMode->PlayerCharacters[i]->GetColumn());

			// If there is a route
			if (Pathfinder->Route.Num() > 2)
			{
				comparisonDistance = 0;
				/** Get Route's 'weight' **/
				for (int32 j = 1; j < Pathfinder->Route.Num()-1; j++)
				{
					comparisonDistance += *Pathfinder->ValidIndexMap.Find(Pathfinder->Route[j]);
				}

				// Check if the route is better than the current route
				if ( (currentDistanceToFocusedCharacter == -1 && comparisonDistance != -1) || (currentDistanceToFocusedCharacter != -1 && comparisonDistance != -1 && comparisonDistance < currentDistanceToFocusedCharacter) )
				{
					// If it is then change movement focus
					EnemyFocus = GameMode->PlayerCharacters[i];
					currentDistanceToFocusedCharacter = comparisonDistance;
					UE_LOG(LogTemp, Warning, TEXT("New distance: %f"), currentDistanceToFocusedCharacter);
				}
			}
		}
		if (EnemyFocus != nullptr && currentDistanceToFocusedCharacter != -1)
		{
			Pathfinder->GetRouteFromIndexes(EnemyFocus->GetRow(), EnemyFocus->GetColumn());
			UE_LOG(LogTemp, Warning, TEXT("'%s' is moving towards '%s' - Route length: %f"), *EnemyName, *EnemyFocus->CharacterAttributes->Stats.CharacterName, currentDistanceToFocusedCharacter);
			ChangeLocation(GetLocationFromIndex(GetFurthestOnRouteIndex()));
		}
		else
		{
			EndTurn();
		}
	}
	else
	{
		ResolveAction();
	}
}

void AAiCharacter::ResolveAction()
{
	ABaseCharacter* targetCharacter = GetNeighbor();
	if (targetCharacter != nullptr)
	{
		OnAttackEnemy(targetCharacter);
	}
	EndTurn();
}

ABaseCharacter* AAiCharacter::GetNeighbor()
{
	/** Get coordinates for this AI character **/
	int32 index;
	FTileProperties tile = Grid->GetTilePropertiesFromTransform(GetActorTransform(), index);
	int32 x = tile.Row;
	int32 y = tile.Column;
	

	if (y - 1 >= 0)
	{
		/** Check left **/
		FTileProperties neighborTile = Grid->GetTilePropertiesFromCoord(x, y - 1);
		if (neighborTile.ActorOnTile != nullptr)
		{
			ABaseCharacter* ActorAsCharacter = Cast<ABaseCharacter>(neighborTile.ActorOnTile);
			if (ActorAsCharacter->bIsPlayerCharacter)
			{
				return ActorAsCharacter;
			}
		}
	}
	if (y + 1 <= Grid->Columns - 1)
	{
		/** Check Right **/
		FTileProperties neighborTile = Grid->GetTilePropertiesFromCoord(x, y + 1);
		if (neighborTile.ActorOnTile != nullptr)
		{
			ABaseCharacter* ActorAsCharacter = Cast<ABaseCharacter>(neighborTile.ActorOnTile);
			if (ActorAsCharacter->bIsPlayerCharacter)
			{
				return ActorAsCharacter;
			}
		}
	}
	if (x + 1 <= Grid->Rows - 1)
	{
		/** Check Top **/
		FTileProperties neighborTile = Grid->GetTilePropertiesFromCoord(x + 1, y);
		if (neighborTile.ActorOnTile != nullptr)
		{
			ABaseCharacter* ActorAsCharacter = Cast<ABaseCharacter>(neighborTile.ActorOnTile);
			if (ActorAsCharacter->bIsPlayerCharacter)
			{
				return ActorAsCharacter;
			}
		}
	}
	if (x - 1 >= 0)
	{
		/** Check Bottom **/
		FTileProperties neighborTile = Grid->GetTilePropertiesFromCoord(x - 1, y);
		if (neighborTile.ActorOnTile != nullptr)
		{
			ABaseCharacter* ActorAsCharacter = Cast<ABaseCharacter>(neighborTile.ActorOnTile);
			if (ActorAsCharacter->bIsPlayerCharacter)
			{
				return ActorAsCharacter;
			}
		}
	}
	if (x + 1 <= Grid->Rows - 1 && y - 1 >= 0)
	{
		/** Check Top-Left **/
		FTileProperties neighborTile = Grid->GetTilePropertiesFromCoord(x + 1, y - 1);
		if (neighborTile.ActorOnTile != nullptr)
		{
			ABaseCharacter* ActorAsCharacter = Cast<ABaseCharacter>(neighborTile.ActorOnTile);
			if (ActorAsCharacter->bIsPlayerCharacter)
			{
				return ActorAsCharacter;
			}
		}
	}
	if (x + 1 <= Grid->Rows - 1 && y + 1 <= Grid->Columns - 1)
	{
		/** Check Top-Right **/
		FTileProperties neighborTile = Grid->GetTilePropertiesFromCoord(x + 1, y + 1);
		if (neighborTile.ActorOnTile != nullptr)
		{
			ABaseCharacter* ActorAsCharacter = Cast<ABaseCharacter>(neighborTile.ActorOnTile);
			if (ActorAsCharacter->bIsPlayerCharacter)
			{
				return ActorAsCharacter;
			}
		}
	}
	if (x - 1 >= 0 && y - 1 >= 0)
	{
		/** Check Bottom-Left **/
		FTileProperties neighborTile = Grid->GetTilePropertiesFromCoord(x - 1, y - 1);
		if (neighborTile.ActorOnTile != nullptr)
		{
			ABaseCharacter* ActorAsCharacter = Cast<ABaseCharacter>(neighborTile.ActorOnTile);
			if (ActorAsCharacter->bIsPlayerCharacter)
			{
				return ActorAsCharacter;
			}
		}
	}
	if (x - 1 >= 0 && y + 1 <= Grid->Columns - 1)
	{
		/** Check Bottom-Right **/
		FTileProperties neighborTile = Grid->GetTilePropertiesFromCoord(x - 1, y + 1);
		if (neighborTile.ActorOnTile != nullptr)
		{
			ABaseCharacter* ActorAsCharacter = Cast<ABaseCharacter>(neighborTile.ActorOnTile);
			if (ActorAsCharacter->bIsPlayerCharacter)
			{
				return ActorAsCharacter;
			}
		}
	}
	return nullptr;
}

FVector AAiCharacter::GetLocationFromIndex(int32 index)
{
	int32 x = index / Grid->Columns;
	int32 y = index % Grid->Columns;
	return FVector((x * Grid->fieldSize) + (Grid->fieldSize / 2), (y * Grid->fieldSize) + (Grid->fieldSize / 2), 50.0f);
}

int32 AAiCharacter::GetFurthestOnRouteIndex()
{
	float speed = CharacterAttributes->Stats.Speed / 5.0f;
	//UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), speed);
	int32 furthestIndex;

	int32 i = Pathfinder->Route.Num()-2; // -1 to adjust for array index and -1 to exclude starting tile
	bool bContinueLoop = true;
	do
	{
		if (Pathfinder->ValidIndexMap.Contains(Pathfinder->Route[i]))
		{
			if (speed >= *Pathfinder->ValidIndexMap.Find(Pathfinder->Route[i]))
			{
				furthestIndex = Pathfinder->Route[i];
			}
		}
		else
		{
			bContinueLoop = false;
		}
		i--;

	} while (bContinueLoop && i >= 1);
	
	//UE_LOG(LogTemp, Warning, TEXT("Furthest Index: %i"), furthestIndex);
	int32 x;
	int32 y;
	Grid->IntexToCoord(furthestIndex, x, y);
	Pathfinder->GetRouteFromIndexes(x, y);
	Mover->RefreshSpline();
	return furthestIndex;
}

void AAiCharacter::OnMove_Implementation()
{
}