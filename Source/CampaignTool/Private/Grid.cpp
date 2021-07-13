// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "../Public/GridField.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

/// <summary>
/// Constructor, set default values here
/// </summary>
AGrid::AGrid()
{
	HISMC_Grid = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HierarchicalInstancedStaticMesh"));
	SetRootComponent(HISMC_Grid);

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
		
		RegisterAllComponents(); //Register all of the components
		
		this->HISMC_Grid->ClearInstances(); //Clear instances and remake them, I guess

		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < Columns; j++)
			{
				FTransform newInstanceTransform;
				newInstanceTransform.SetLocation(FVector((i*fieldSize)+(fieldSize/2), (j * fieldSize) + (fieldSize / 2), 0));
				HISMC_Grid->AddInstance(newInstanceTransform);
			}
		}
	}
	
}