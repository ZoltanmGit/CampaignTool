// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

/// <summary>
/// Constructor, set default values here
/// </summary>
AGrid::AGrid()
{
	HISMC_Grid = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HierarchicalInstancedStaticMesh"));
	HISMC_Grid->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //Does not react to physics, only overlaps, traces , etc...
	HISMC_Grid->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	SetRootComponent(HISMC_Grid); // Set the instancedMesh as the rootcomponent for this class
	Rows = 10;
	Columns = 8;
	fieldSize = 100;
}

/// <summary>
/// Called when the game starts or when spawned
/// </summary>
void AGrid::BeginPlay()
{
	Super::BeginPlay();
}

/// <summary>
/// Called at every property change in the editor
/// We use this to drag the grid in the editor
/// </summary>
/// <param name="Transform">An FTransform as parameter</param>
void AGrid::OnConstruction(const FTransform& Transform) {

	if (HISMC_Grid && HISMC_Grid->GetStaticMesh())
	{
		
		RegisterAllComponents(); //Register all of the components - we register components if they are dynamically created at runtime
		
		this->HISMC_Grid->ClearInstances(); //Clear instances and remake them
		this->GridDataArray.Empty();

		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < Columns; j++)
			{
				FTransform newInstanceTransform;
				newInstanceTransform.SetLocation(FVector((i*fieldSize)+(fieldSize/2), (j * fieldSize) + (fieldSize / 2), 0));
				FTileProperties newTileData = FTileProperties();
				newTileData.bIsObscured = true;
				newTileData.TerrainType = TerrainType::Dirt;
				newTileData.LightType = LightType::Bright;
				GridDataArray.Add(newTileData);
				HISMC_Grid->AddInstance(newInstanceTransform);
			}
		}
	}
}