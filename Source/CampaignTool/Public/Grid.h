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
		FTileProperties GetTilePropertiesFromCoord(int32 Row, int32 Column);

	void SpawnInstance(int32 x, int32 y, int32 TerrainInt);

	void InitializeGrid(int32 argRows,int32 argColumns,TArray<int32> Array);
private:
	/// <summary>
	/// This function is used to get a valid TerrainType from an integer
	/// It clamps the value between -1 and the number of initialized tiles
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	TerrainType GetTerrainTypeFromInt(int value);
public:
	virtual void OnConstruction(const FTransform& transform);

	UFUNCTION(BlueprintCallable)
		int32 CoordToIndex(int32 x, int32 y);
	UFUNCTION(BlueprintCallable)
		void IntexToCoord(int32 Index, int32 &OutX, int32 &OutY);
};
