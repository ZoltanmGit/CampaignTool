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
		newHISMC->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Overlap); //Overlap so the camera springarm doesn't shorten itself with strange collision
		newHISMC->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
		newHISMC->SetMobility(EComponentMobility::Static);
		newHISMC->bCastContactShadow = false;
		newHISMC->CastShadow = false;
		newHISMC->SetupAttachment(GridRootComponent);
		HISMC_GridArray.Add(newHISMC);
	}
	fieldSize = 100;
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
	OutIndex = (((tileTransform.GetLocation().X - 50) / fieldSize) * Columns) + ((tileTransform.GetLocation().Y - 50) / fieldSize);;
	return GridDataArray[OutIndex];
}

FTileProperties AGrid::GetTilePropertiesFromIndex(int32 Row, int32 Column)
{
	int32 index = (Row * Columns) + Column;
	return GridDataArray[index];
}

void AGrid::SpawnInstance(int x, int y, int TerrainInt)
{

	//UE_LOG(LogTemp, Warning, TEXT("SpawnInstance: x:%i y:%i Type:%i"), x, y, TerrainInt);
	FTileProperties newTileData = FTileProperties();
	newTileData.Row = x;
	newTileData.Column = y;
	newTileData.LightType = LightType::Bright;
	newTileData.bIsObscured = false;
	newTileData.bIsOccupied = false;
	newTileData.ActorOnTile = nullptr;

	if (TerrainInt > -1)
	{
		FTransform newInstanceTransform;
		newInstanceTransform.SetLocation(FVector((x * fieldSize) + (fieldSize / 2), (y * fieldSize) + (fieldSize / 2), 0.0f));
		if (HISMC_GridArray[TerrainInt] && HISMC_GridArray[TerrainInt]->GetStaticMesh())
		{

			newTileData.TerrainType = GetTerrainTypeFromInt(TerrainInt);
			HISMC_GridArray[TerrainInt]->AddInstance(newInstanceTransform);
		}
	}
	else if (TerrainInt == -1)
	{
		newTileData.TerrainType = TerrainType::Void;
	}
	GridDataArray.Add(newTileData);
}

void AGrid::InitializeGrid(int argRows, int argColumns, int argGrid[])
{
	if(argRows > 0 && argColumns > 0 && argGrid != nullptr)
	{
		Rows = argRows;
		Columns = argColumns;
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < Columns; j++)
			{
				SpawnInstance(i,j,argGrid[(i*Columns)+j]);
			}
		}
	}
}
TerrainType AGrid::GetTerrainTypeFromInt(int value)
{
	switch (FMath::Clamp(value,-1,this->HISMC_GridArray.Num()))
	{
	case 0:
		return TerrainType::Grass;
		break;
	case 1:
		return TerrainType::Water;
		break;
	case 2:
		return TerrainType::DeepWater;
		break;
	case 3:
		return TerrainType::Wood;
		break;
	case 4:
		return TerrainType::Dirt;
		break;
	case 5:
		return TerrainType::Rock;
		break;
	case 6 :
		return TerrainType::Grass;
		break;
	default:
		return TerrainType::Void;
		break;
	};
}

/// <summary>
/// Called at every property change in the editor
/// We use this to drag the grid in the editor
/// </summary>
/// <param name="Transform">An FTransform as parameter</param>
void AGrid::OnConstruction(const FTransform& Transform) 
{
	
}