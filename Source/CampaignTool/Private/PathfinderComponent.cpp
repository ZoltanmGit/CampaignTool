// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfinderComponent.h"
#include "../Public/BaseCharacter.h"
#include "../Public/Grid.h"
#include "../Public/PathfindingStructure.h"

// Sets default values for this component's properties
UPathfinderComponent::UPathfinderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}
int32 UPathfinderComponent::CoordToIndex(int32 x, int32 y)
{
	return (x * MapSize) + y;
}

void UPathfinderComponent::ProcessNode(int32 x, int32 y, int32 range)
{
	UE_LOG(LogTemp, Warning, TEXT("Processing %i and %i"), x, y);
	FDijkstraNode CurrentNode = DijkstraGrid[CoordToIndex(x, y)];
	bool bQueueNeighbors = true;
	if (FMath::CeilToFloat(CurrentNode.NodeValue) == range)
	{
		bQueueNeighbors = false;
		UE_LOG(LogTemp, Warning, TEXT("x:%i  y:%i don't queue neighbours"), CurrentNode.x,CurrentNode.y);
	}
	//Left
	if (y - 1 >= 0)
	{
		
		if (DijkstraGrid[CoordToIndex(x, y - 1)].NodeValue > CurrentNode.NodeValue + 1.0f && DijkstraGrid[CoordToIndex(x, y - 1)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x, y - 1)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x, y - 1)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x, y - 1);
				DijkstraQ.Enqueue(pair);
			}
		}
	}
	//Right
	if (y + 1  <= MapSize-1)
	{
		
		if (DijkstraGrid[CoordToIndex(x, y + 1)].NodeValue > CurrentNode.NodeValue + 1.0f && DijkstraGrid[CoordToIndex(x, y + 1)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x, y + 1)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x, y + 1)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x, y + 1);
				DijkstraQ.Enqueue(pair);
			}
		}
	}

	// Top
	if (x + 1 <= MapSize - 1)
	{
		
		if (DijkstraGrid[CoordToIndex(x+1, y)].NodeValue > CurrentNode.NodeValue + 1.0f && DijkstraGrid[CoordToIndex(x + 1, y)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x + 1, y)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x+1, y)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x+1, y);
				DijkstraQ.Enqueue(pair);
			}
		}
	}

	// Bottom
	if (x - 1 >= 0)
	{
		
		if (DijkstraGrid[CoordToIndex(x - 1, y)].NodeValue > CurrentNode.NodeValue + 1.0f && DijkstraGrid[CoordToIndex(x - 1, y)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x - 1, y)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x - 1, y)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y);
				DijkstraQ.Enqueue(pair);
			}
		}
	}

	// Top-Left
	if (x + 1 <= MapSize - 1 && y-1 >= 0)
	{
		
		if (DijkstraGrid[CoordToIndex(x + 1, y-1)].NodeValue > CurrentNode.NodeValue + 1.5f && DijkstraGrid[CoordToIndex(x + 1, y - 1)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x + 1, y - 1)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x + 1, y-1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f < range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y-1);
				DijkstraQ.Enqueue(pair);
			}
		}
	}

	// Top-Right
	if (x + 1 <= MapSize - 1 && y + 1 <= MapSize - 1)
	{
		
		if (DijkstraGrid[CoordToIndex(x + 1, y + 1)].NodeValue > CurrentNode.NodeValue + 1.5f&& DijkstraGrid[CoordToIndex(x + 1, y + 1)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x + 1, y + 1)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x + 1, y + 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f < range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y+1);
				DijkstraQ.Enqueue(pair);
			}
		}
	}

	// Bottom-Left
	if (x - 1 >=0 && y - 1 >= 0)
	{
		
		if (DijkstraGrid[CoordToIndex(x - 1, y - 1)].NodeValue > CurrentNode.NodeValue + 1.5f&& DijkstraGrid[CoordToIndex(x - 1, y - 1)].bIsValidTerrain &&!DijkstraGrid[CoordToIndex(x - 1, y - 1)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x - 1, y - 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f < range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y-1);
				DijkstraQ.Enqueue(pair);
			}
		}
	}
	// Bottom-Right
	if (x - 1 >= 0 && y + 1 <= MapSize -1)
	{
		
		if (DijkstraGrid[CoordToIndex(x - 1, y + 1)].NodeValue > CurrentNode.NodeValue + 1.5f&& DijkstraGrid[CoordToIndex(x - 1, y + 1)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x - 1, y + 1)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x - 1, y + 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f < range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y+1);
				DijkstraQ.Enqueue(pair);
			}
		}
	}
	DijkstraGrid[CoordToIndex(x, y)].bWasProcessed = true;
	ValidMovementArray.Add(CoordToIndex(x, y));
}

void UPathfinderComponent::GetValidMovementIndexes(int32 x, int32 y, int32 range)
{
	//Get the grid from the parent
	ABaseCharacter* Owner = Cast<ABaseCharacter>(this->GetOwner());
	if (Owner != nullptr)
	{
		DijkstraGrid.Empty();
		ValidMovementArray.Empty();

		for (int32 i = 0; i < MapSize; i++)
		{
			for (int32 j = 0; j < MapSize; j++)
			{
				FDijkstraNode node;
				node.x = i;
				node.y = j;
				node.bWasProcessed = false;
				if (Owner->Grid->GetTilePropertiesFromIndex(i, j).bIsOccupied || Owner->Grid->GetTilePropertiesFromIndex(i, j).TerrainType == TerrainType::Water || Owner->Grid->GetTilePropertiesFromIndex(i, j).TerrainType == TerrainType::DeepWater || Owner->Grid->GetTilePropertiesFromIndex(i, j).TerrainType == TerrainType::Void)
				{
					node.bIsValidTerrain = false;
				}
				else
				{
					node.bIsValidTerrain = true;
				}

				if (x == i && y == j)
				{
					node.NodeValue = 0;
				}
				else
				{
					node.NodeValue = MapSize * MapSize + 1;
				}
				DijkstraGrid.Add(node);
			}
		}
		TPair<int32, int32> asd = TPair<int32, int32>(x, y);
		DijkstraQ.Enqueue(asd);
		while (DijkstraQ.IsEmpty() == false)
		{
			TPair<int32, int32> pair;
			DijkstraQ.Dequeue(pair);
			ProcessNode(pair.Key,pair.Value, range);
		}
		/*TPair<int32, int32> pair;
		DijkstraQ.Dequeue(pair);
		ProcessNode(pair.Key, pair.Value, range);*/


		//UE_LOG(LogTemp, Warning, TEXT("Finished pathfinding"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pathfinder didn't find owner."));
	}
}

