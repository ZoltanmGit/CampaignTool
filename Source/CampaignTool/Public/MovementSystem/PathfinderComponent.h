// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementSystem/PathfindingStructure.h"
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
	
	void ProcessNodeForRoute(int32 x, int32 y);
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
	
	TArray<int32> Route;

public:
	UFUNCTION(BlueprintCallable)
		void GetValidMovementIndexes(int32 x, int32 y, float range);
	
	UFUNCTION(BlueprintCallable)
		TArray<int32> GetRouteFromIndexes(int32 x, int32 y);

	void EmptyProperties();
};
