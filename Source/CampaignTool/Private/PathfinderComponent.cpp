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

/*void UPathfinderComponent::ProcessNode(Node node)
{
}*/

int32 UPathfinderComponent::CoordToIndex(int32 x, int32 y)
{
	return (x * MapSize) + y;
}

TArray<int32> UPathfinderComponent::GetValidMovementIndexes(int32 x, int32 y, int32 range)
{
	//Get the grid from the parent
	ABaseCharacter* Owner = Cast<ABaseCharacter>(this->GetOwner());
	if (Owner != nullptr)
	{
		DijkstraGrid.Empty();

		for (int32 i = 0; i < MapSize; i++)
		{
			for (int32 j = 0; j < MapSize; j++)
			{
				FDijkstraNode node;
				node.x = i;
				node.y = j;
				node.bWasProcessed = false;
				if (Owner->Grid->GetTilePropertiesFromIndex(i, j).bIsOccupied || Owner->Grid->GetTilePropertiesFromIndex(i, j).TerrainType == TerrainType::Water)
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
		return TArray<int32>();
	}
	UE_LOG(LogTemp, Warning, TEXT("Pathfinder didn't find owner."));
	return TArray<int32>();
}

