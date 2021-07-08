// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	InstancedTileMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedTileMesh"));
	InstancedTileMesh->SetMobility(EComponentMobility::Static);
	InstancedTileMesh->CastShadow = false;
	InstancedTileMesh->SetStaticLightingMapping(true, 8);
	RootComponent = InstancedTileMesh;

	gridSize = 10;
	fieldSize = 100;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AGrid::SpawnGrid()
{
	if (InstancedTileMesh->GetStaticMesh() != nullptr)
	{
		int adjustment = fieldSize / 2;
		FVector spawnLocation;
		for (int i = 0; i < gridSize; i++)
		{
			for (int j = 0; j < gridSize; j++)
			{
				spawnLocation = FVector((i * fieldSize) + adjustment, (j * fieldSize) + adjustment, adjustment);
				FTransform transform;
				transform.SetLocation(spawnLocation);
				InstancedTileMesh->AddInstance(transform);
			}
		}
	}
}
