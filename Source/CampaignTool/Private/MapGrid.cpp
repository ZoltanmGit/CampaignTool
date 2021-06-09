// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGrid.h"
#include "../Public/GridField.h"
#include "Actor.h"

// Sets default values
AMapGrid::AMapGrid()
{
	gridSize = 4;
	fieldSize = 150;
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
		grid.Init(nullptr, gridSize * gridSize);
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;//Grid is the owner of the GridFields

		int adjustment = fieldSize / 2;
		FVector spawnLocation;

		int index = 0;
		for (int i = 0; i < gridSize; i++)
		{
			for (int j = 0; j < gridSize; j++)
			{
				spawnLocation = FVector((i * fieldSize)+adjustment, (j * fieldSize)+adjustment, adjustment);
				grid[index] = GetWorld()->SpawnActor<AGridField>(FieldToSpawn, spawnLocation, FRotator(0.0f), spawnParams);
				grid[index]->SetActorRelativeLocation(spawnLocation);
				grid[index]->Row = i;
				grid[index]->Column = j;
				grid[index]->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				index++;
			}
		}
	}
}

