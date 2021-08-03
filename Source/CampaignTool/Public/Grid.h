// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"


UENUM(BlueprintType)
enum TerrainType
{
	Grass UMETA(DisplayName = "Grass"),
	Water UMETA(DisplayName = "Water"),
	Dirt  UMETA(DisplayName = "Dirt"),
	Rock  UMETA(DisplayName = "Rock")
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
	bool bIsObscured;
	TEnumAsByte<TerrainType> TerrainType;
	TEnumAsByte<LightType> LightType;
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
	
public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GridProperties)
		class UHierarchicalInstancedStaticMeshComponent* HISMC_Grid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		TArray<FTileProperties> GridDataArray;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GridProperties)
		int Rows;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GridProperties)
		int Columns;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		int fieldSize;

public:
	virtual void OnConstruction(const FTransform& transform);
};
