// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGrid.generated.h"

UCLASS()
class CAMPAIGNTOOL_API AMapGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Properties)
		float gridSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Properties)
		float fieldSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fields)
		TSubclassOf<class AGridField> FieldToSpawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fields)
		TArray<class AGridField*> grid;
protected:
	UFUNCTION(BlueprintCallable)
		void SpawnGrid();
};
