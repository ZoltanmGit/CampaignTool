// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Public/PathfindingStructure.h"
#include "Components/ActorComponent.h"
#include "PathfinderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMPAIGNTOOL_API UPathfinderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfinderComponent();

protected:
	//Functions
	int32 CoordToIndex(int32 x, int32 y);
	void ProcessNode(int32 x, int32 y, float range);
public:
	//Properties
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		int32 Rows;
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		int32 Columns;
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		TArray<FDijkstraNode> DijkstraGrid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pathfinding)
		TMap<int32, float> ValidIndexMap;

	TQueue<TPair<int32, int32>> DijkstraQ;

public:
	UFUNCTION(BlueprintCallable)
		void GetValidMovementIndexes(int32 x, int32 y, float range); //Returns the grid's dataArray indexes of valid movement points
	void EmptyProperties();
};
