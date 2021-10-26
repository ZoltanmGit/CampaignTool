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
	return (x * Columns) + y;
}

void UPathfinderComponent::ProcessNode(int32 x, int32 y, float range)
{
	FDijkstraNode CurrentNode = DijkstraGrid[CoordToIndex(x, y)];
	bool bQueueNeighbors = true;
	if (FMath::CeilToFloat(CurrentNode.NodeValue) == range)
	{
		bQueueNeighbors = false;
		//UE_LOG(LogTemp, Warning, TEXT("x:%i  y:%i don't queue neighbours range:%f"), CurrentNode.x,CurrentNode.y, range);
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
	if (y + 1  <= Columns-1)
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
	if (x + 1 <= Rows - 1)
	{
		if (DijkstraGrid[CoordToIndex(x + 1, y)].NodeValue > CurrentNode.NodeValue + 1.0f && DijkstraGrid[CoordToIndex(x + 1, y)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x + 1, y)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x + 1, y)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y);
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
	if (x + 1 <= Rows - 1 && y-1 >= 0)
	{
		if (DijkstraGrid[CoordToIndex(x + 1, y - 1)].NodeValue > CurrentNode.NodeValue + 1.5f && DijkstraGrid[CoordToIndex(x + 1, y - 1)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x + 1, y - 1)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x + 1, y - 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y - 1);
				DijkstraQ.Enqueue(pair);
			}
		}
	}

	// Top-Right
	if (x + 1 <= Rows - 1 && y + 1 <= Columns - 1)
	{
		if (DijkstraGrid[CoordToIndex(x + 1, y + 1)].NodeValue > CurrentNode.NodeValue + 1.5f && DijkstraGrid[CoordToIndex(x + 1, y + 1)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x + 1, y + 1)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x + 1, y + 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y + 1);
				DijkstraQ.Enqueue(pair);
			}
		}
	}

	// Bottom-Left
	if (x - 1 >=0 && y - 1 >= 0)
	{
		if (DijkstraGrid[CoordToIndex(x - 1, y - 1)].NodeValue > CurrentNode.NodeValue + 1.5f && DijkstraGrid[CoordToIndex(x - 1, y - 1)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x - 1, y - 1)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x - 1, y - 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y - 1);
				DijkstraQ.Enqueue(pair);
			}
		}
	}
	// Bottom-Right
	if (x - 1 >= 0 && y + 1 <= Columns -1)
	{
		
		if (DijkstraGrid[CoordToIndex(x - 1, y + 1)].NodeValue > CurrentNode.NodeValue + 1.5f && DijkstraGrid[CoordToIndex(x - 1, y + 1)].bIsValidTerrain && !DijkstraGrid[CoordToIndex(x - 1, y + 1)].bWasProcessed)
		{
			DijkstraGrid[CoordToIndex(x - 1, y + 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y + 1);
				DijkstraQ.Enqueue(pair);
			}
		}
	}
	DijkstraGrid[CoordToIndex(x, y)].bWasProcessed = true;
	ValidIndexMap.Add(CoordToIndex(x, y), DijkstraGrid[CoordToIndex(x, y)].NodeValue);
	
}

void UPathfinderComponent::ProcessNodeForRoute(int32 x, int32 y)
{
	FDijkstraNode CurrenetNode = DijkstraGrid[CoordToIndex(x, y)];
	TPair<int32, int32> Coord = TPair<int32, int32>();
	Coord.Key = x;
	Coord.Value = y;
	//Left
	if (y - 1 >= 0)
	{
		if (DijkstraGrid[CoordToIndex(x, y - 1)].NodeValue < DijkstraGrid[CoordToIndex(Coord.Key, Coord.Value)].NodeValue)
		{
			Coord.Key = x;
			Coord.Value = y - 1;
		}
	}
	//Right
	if (y + 1 <= Columns - 1)
	{
		if (DijkstraGrid[CoordToIndex(x, y + 1)].NodeValue < DijkstraGrid[CoordToIndex(Coord.Key, Coord.Value)].NodeValue)
		{
			Coord.Key = x;
			Coord.Value = y + 1;
		}
	}

	// Top
	if (x + 1 <= Rows - 1)
	{
		if (DijkstraGrid[CoordToIndex(x+1, y)].NodeValue < DijkstraGrid[CoordToIndex(Coord.Key, Coord.Value)].NodeValue)
		{
			Coord.Key = x+1;
			Coord.Value = y;
		}
	}

	// Bottom
	if (x - 1 >= 0)
	{
		if (DijkstraGrid[CoordToIndex(x - 1, y)].NodeValue < DijkstraGrid[CoordToIndex(Coord.Key, Coord.Value)].NodeValue)
		{
			Coord.Key = x - 1;
			Coord.Value = y;
		}
	}

	// Top-Left
	if (x + 1 <= Rows - 1 && y - 1 >= 0)
	{
		if (DijkstraGrid[CoordToIndex(x + 1, y-1)].NodeValue < DijkstraGrid[CoordToIndex(Coord.Key, Coord.Value)].NodeValue)
		{
			Coord.Key = x + 1;
			Coord.Value = y - 1;
		}
	}

	// Top-Right
	if (x + 1 <= Rows - 1 && y + 1 <= Columns - 1)
	{
		if (DijkstraGrid[CoordToIndex(x + 1, y + 1)].NodeValue < DijkstraGrid[CoordToIndex(Coord.Key, Coord.Value)].NodeValue)
		{
			Coord.Key = x + 1;
			Coord.Value = y + 1;
		}
	}

	// Bottom-Left
	if (x - 1 >= 0 && y - 1 >= 0)
	{
		if (DijkstraGrid[CoordToIndex(x - 1 , y - 1)].NodeValue < DijkstraGrid[CoordToIndex(Coord.Key, Coord.Value)].NodeValue)
		{
			Coord.Key = x - 1;
			Coord.Value = y - 1;
		}
	}
	// Bottom-Right
	if (x - 1 >= 0 && y + 1 <= Columns - 1)
	{
		if (DijkstraGrid[CoordToIndex(x - 1, y + 1)].NodeValue < DijkstraGrid[CoordToIndex(Coord.Key, Coord.Value)].NodeValue)
		{
			Coord.Key = x - 1;
			Coord.Value = y + 1;
		}
	}
	Route.Add(CoordToIndex(Coord.Key, Coord.Value));
	if (DijkstraGrid[CoordToIndex(Coord.Key, Coord.Value)].NodeValue != 0.0f)
	{
		ProcessNodeForRoute(Coord.Key, Coord.Value);
	}
}

void UPathfinderComponent::GetValidMovementIndexes(int32 x, int32 y, float range)
{
	//Get the grid from the parent
	ABaseCharacter* Owner = Cast<ABaseCharacter>(this->GetOwner());
	if (Owner != nullptr)
	{
		DijkstraGrid.Empty();
		ValidIndexMap.Empty();

		for (int32 i = 0; i < Rows; i++)
		{
			for (int32 j = 0; j < Columns; j++)
			{
				FDijkstraNode node;
				node.x = i;
				node.y = j;
				node.bWasProcessed = false;
				if (Owner->Grid->GetTilePropertiesFromCoord(i, j).bIsOccupied || Owner->Grid->GetTilePropertiesFromCoord(i, j).TerrainType == TerrainType::Water || Owner->Grid->GetTilePropertiesFromCoord(i, j).TerrainType == TerrainType::DeepWater || Owner->Grid->GetTilePropertiesFromCoord(i, j).TerrainType == TerrainType::Void)
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
					node.NodeValue = Rows * Columns + 1;
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
		ValidIndexMap.Remove(CoordToIndex(x, y)); //Remove the current position
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pathfinder didn't find owner."));
	}
}

TArray<int32> UPathfinderComponent::GetRouteFromIndexes(int32 x, int32 y)
{
	if (Route.Num() > 0)
	{
		Route.Empty();
	}
	Route.Add(CoordToIndex(x, y));
	ProcessNodeForRoute(x, y);
	return Route;
}

void UPathfinderComponent::EmptyProperties()
{
	if (!DijkstraQ.IsEmpty())
	{
		DijkstraQ.Empty();
	}
	if (DijkstraGrid.Num() > 0)
	{
		DijkstraGrid.Empty();
	}
	if (ValidIndexMap.Num() > 0)
	{
		ValidIndexMap.Empty();
	}
}

