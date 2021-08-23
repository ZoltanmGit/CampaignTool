// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldStructure.h"
#include "Grid.generated.h"

UCLASS(BlueprintType, DefaultToInstanced)
class CAMPAIGNTOOL_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* GridRootComponent;

	// 0 - Green, 1 - Blue, 2 - Brown, 3 - Grey
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GridProperties)
		TArray<class UHierarchicalInstancedStaticMeshComponent*> HISMC_GridArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		TArray<FTileProperties> GridDataArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		int32 fieldSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		int32 Rows;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		int32 Columns;

public:
	UFUNCTION(BlueprintCallable)
		FTileProperties GetTilePropertiesFromTransform(const FTransform tileTransform, int32 &OutIndex);
	UFUNCTION(BlueprintCallable)
		FTileProperties GetTilePropertiesFromIndex(int32 Row, int32 Column);

	void SpawnInstance(int x, int y, int TerrainInt);

	void InitializeGrid(int argRows,int argColumns,int argGrid[]);
private:
	TerrainType GetTerrainTypeFromInt(int value);
public:
	virtual void OnConstruction(const FTransform& transform);
};
