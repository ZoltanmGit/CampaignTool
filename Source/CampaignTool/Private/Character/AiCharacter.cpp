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
	
	int32 index;//Unused
	FTileProperties tile = Grid->GetTilePropertiesFromTransform(this->GetActorTransform(), index);
	Pathfinder->GetValidMovementIndexes(tile.Row, tile.Column, 100);

	ACampaignToolGameModeBase* GameMode = Cast<ACampaignToolGameModeBase>(GetWorld()->GetAuthGameMode());

	

	/** Determinte Focus based on who's closest **/

	int32 playerIndex;
	FTileProperties playerTile = Grid->GetTilePropertiesFromTransform(GameMode->PlayerCharacters[0]->GetActorTransform(), playerIndex);
	
	
	Pathfinder->GetRouteFromIndexes(GameMode->PlayerCharacters[0]->GetRow(), GameMode->PlayerCharacters[0]->GetColumn());
	int32 distanceToFocusedCharacter = Pathfinder->Route.Num();
	
	if (Pathfinder->Route.Num() > 2 && Pathfinder->Route.Num()!=0)
	{
		EnemyFocus = GameMode->PlayerCharacters[0];
		distanceToFocusedCharacter = Pathfinder->Route.Num();
	}

	for (int32 i = 1; i < GameMode->PlayerCharacters.Num(); i++)
	{
		playerIndex;
		playerTile = Grid->GetTilePropertiesFromTransform(GameMode->PlayerCharacters[i]->GetActorTransform(), playerIndex);
		//if (Pathfinder->Route.Num() < 1) CHECK IF HAS NEIGHBOUR
		//{
		Pathfinder->GetRouteFromIndexes(GameMode->PlayerCharacters[i]->GetRow(), GameMode->PlayerCharacters[i]->GetColumn());

		if (Pathfinder->Route.Num() < distanceToFocusedCharacter && Pathfinder->Route.Num() > 2 && Pathfinder->Route.Num() != 0)
		{
			EnemyFocus = GameMode->PlayerCharacters[i];
			distanceToFocusedCharacter = Pathfinder->Route.Num();
		}
		//}
	}


	if (EnemyFocus != nullptr && distanceToFocusedCharacter != -1)
	{
		Pathfinder->GetRouteFromIndexes(EnemyFocus->GetRow(), EnemyFocus->GetColumn());
		UE_LOG(LogTemp, Warning, TEXT("'%s' is moving towards '%s' - Route length: %i"), *EnemyName, *EnemyFocus->CharacterAttributes->Stats.CharacterName, Pathfinder->Route.Num());
		ChangeLocation(GetLocationFromIndex(GetFurthestOnRouteIndex()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("'%s' is not moving."), *EnemyName);
		ResolveAction();
	}
}

void AAiCharacter::ResolveAction()
{
	EndTurn();
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