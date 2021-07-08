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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GridProperties)
		UInstancedStaticMeshComponent* InstancedTileMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GridProperties)
		int gridSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GridProperties)
		int fieldSize;

	UFUNCTION(BlueprintCallable)
		void SpawnGrid();
};
