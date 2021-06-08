// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGrid.h"
#include "../Public/GridField.h"

// Sets default values
AMapGrid::AMapGrid()
{
	gridSize = 4;
	fieldSize = 150;
	grid.Init(nullptr, gridSize * gridSize);
}

// Called when the game starts or when spawned
void AMapGrid::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnGrid();
}

void AMapGrid::SpawnGrid()
{
	if (FieldToSpawn)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;//Grid is the owner of the GridFields

		FVector spawnLocation = this->GetActorLocation(); //Starts off at the actor's location

		int index = 0;
		for (int i = 0; i < gridSize; i++)
		{
			for (int j = 0; j < gridSize; j++)
			{
				spawnLocation = FVector(i * fieldSize, j * fieldSize, spawnLocation.Z);
				grid[index] = GetWorld()->SpawnActor<AGridField>(FieldToSpawn, spawnLocation, FRotator(0.0f), spawnParams);
				grid[index]->Row = i;
				grid[index]->Column = j;
				index++;
			}
		}
	}
}

