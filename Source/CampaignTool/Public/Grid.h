// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	/*UPROPERTY(EditAnywhere, EditInline, BlueprintReadOnly, Category = GridProperties)
		TArray<class AGridField*> RepGrid;*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GridProperties)
		class UHierarchicalInstancedStaticMeshComponent* HISMC_Grid;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GridProperties)
		int Rows;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GridProperties)
		int Columns;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		int fieldSize;

public:
	virtual void OnConstruction(const FTransform& transform);
};
