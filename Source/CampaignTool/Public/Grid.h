// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"


UENUM(BlueprintType)
enum TerrainType
{
	Grass UMETA(DisplayName = "Grass"), //0
	Water UMETA(DisplayName = "Water"), //1
	Wood  UMETA(DisplayName = "Wood"),  //2
	Dirt  UMETA(DisplayName = "Dirt"),  //3
	Rock  UMETA(DisplayName = "Rock")   //4
};
UENUM(BlueprintType)
enum LightType
{
	Bright UMETA(DisplayName = "Bright"),
	Dim UMETA(DisplayName = "Dim"),
	Darkness  UMETA(DisplayName = "Darkness"),
};

USTRUCT(BlueprintType)
struct FTileProperties
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
		bool bIsObscured;
	UPROPERTY(VisibleAnywhere)
		bool bIsOccupied;
	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<TerrainType> TerrainType;
	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<LightType> LightType;
	UPROPERTY(VisibleAnywhere)
		int Row;
	UPROPERTY(VisibleAnywhere)
		int Column;
};

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

	int tempMatrix[100] = { 0,0,0,0,1,1,0,0,0,0,
							0,0,0,0,1,1,0,0,0,0,
							0,0,0,2,2,2,2,0,0,0,
							0,0,0,2,2,2,2,0,0,0,
							0,0,0,2,2,2,2,0,0,0,
							0,0,0,0,1,1,0,0,0,0,
							0,0,0,0,1,1,0,0,0,0,
							0,0,0,0,1,1,0,0,0,0,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1
	};
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* GridRootComponent;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GridProperties)
		class UHierarchicalInstancedStaticMeshComponent* HISMC_Grid;*/

	// 0 - Green, 1 - Blue, 2 - Brown, 3 - Grey
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GridProperties)
		TArray<class UHierarchicalInstancedStaticMeshComponent*> HISMC_GridArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		TArray<FTileProperties> GridDataArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		int32 fieldSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		int32 MapSize;

	UFUNCTION(BlueprintCallable)
		FTileProperties GetTilePropertiesFromTransform(const FTransform tileTransform, int32 &OutIndex);

	UFUNCTION(BlueprintCallable)
		void SpawnInstance(const int row, const int column);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ApplyTileMaterial(const int tileIndex, const float tileType);
public:
	virtual void OnConstruction(const FTransform& transform);
};
