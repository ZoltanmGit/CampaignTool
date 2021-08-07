// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"

/// <summary>
/// Constructor, set default values here
/// </summary>
AGrid::AGrid()
{
	GridRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("GridRoot"));
	GridRootComponent->SetWorldLocation(FVector(0.0f,0.0f ,0.0f ));
	GridRootComponent->SetMobility(EComponentMobility::Static);
	SetRootComponent(GridRootComponent);

	for (int i = 0; i < 7; i++)
	{
		FName newHISMCName = *FString::Printf(TEXT("HISMC_%i"), i);
		UHierarchicalInstancedStaticMeshComponent* newHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(newHISMCName);
		newHISMC->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //Does not react to physics, only overlaps, traces , etc...
		newHISMC->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block); // Only blocks the selection channel
		newHISMC->SetMobility(EComponentMobility::Static);
		newHISMC->bCastContactShadow = false;
		newHISMC->CastShadow = false;
		newHISMC->SetupAttachment(GridRootComponent);
		HISMC_GridArray.Add(newHISMC);
	}
	fieldSize = 100;
	MapSize = 10;
}

/// <summary>
/// Called when the game starts or when spawned
/// </summary>
void AGrid::BeginPlay()
{
	Super::BeginPlay();
}

FTileProperties AGrid::GetTilePropertiesFromTransform(const FTransform tileTransform, int32& OutIndex)
{
	int32 index = (((tileTransform.GetLocation().X - 50) / fieldSize) * MapSize) + ((tileTransform.GetLocation().Y - 50) / fieldSize);
	return GridDataArray[index];
}

void AGrid::SpawnInstance(const int row, const int column)
{
}

void AGrid::ApplyTileMaterial_Implementation(const int tileIndex, const float tileType)
{

}

/// <summary>
/// Called at every property change in the editor
/// We use this to drag the grid in the editor
/// </summary>
/// <param name="Transform">An FTransform as parameter</param>
void AGrid::OnConstruction(const FTransform& Transform) {
	bool bValid = true; 
	for (int i = 0; i < 3; i++)
	{
		if (!HISMC_GridArray[i] || !HISMC_GridArray[i]->GetStaticMesh())
		{
			bValid = false;
		}
		else
		{
			HISMC_GridArray[i]->ClearInstances();
		}
	}

	GridDataArray.Empty();

	if (bValid)
	{
		RegisterAllComponents();

		for (int i = 0; i < MapSize; i++)
		{
			for (int j = 0; j < MapSize; j++)
			{
				if (tempMatrix[((i * MapSize) + j)] >= 0)
				{
					FTransform newInstanceTransform;
					newInstanceTransform.SetLocation(FVector((i * fieldSize) + (fieldSize / 2), (j * fieldSize) + (fieldSize / 2), 0));

					FTileProperties newTileData = FTileProperties();
					newTileData.bIsObscured = false;
					newTileData.bIsOccupied = false;
					newTileData.Row = i;
					newTileData.Column = j;
					newTileData.LightType = LightType::Bright;
					switch (tempMatrix[((i * 10) + j)])
					{
					case 0:
						newTileData.TerrainType = TerrainType::Grass;
						HISMC_GridArray[0]->AddInstance(newInstanceTransform);
						break;
					case 1:
						newTileData.TerrainType = TerrainType::Water;
						HISMC_GridArray[1]->AddInstance(newInstanceTransform);
						break;
					case 2:
						newTileData.TerrainType = TerrainType::Wood;
						HISMC_GridArray[2]->AddInstance(newInstanceTransform);
						break;
					case 3:
						newTileData.TerrainType = TerrainType::Grass;
						//HISMC_GridArray[0]->AddInstance(newInstanceTransform);
						break;
					case 4:
						newTileData.TerrainType = TerrainType::Grass;
						//HISMC_GridArray[0]->AddInstance(newInstanceTransform);
						break;
					default:
						break;
					}
					GridDataArray.Add(newTileData);

				}
			}
		}

	}

	/*if (HISMC_Grid && HISMC_Grid->GetStaticMesh())
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
				newTileData.Row = i;
				newTileData.Column = j;
				GridDataArray.Add(newTileData);
				HISMC_Grid->AddInstance(newInstanceTransform);
			}
		}
	}*/
}