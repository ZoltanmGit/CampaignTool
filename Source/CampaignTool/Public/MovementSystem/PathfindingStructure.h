#pragma once

#include "CoreMinimal.h"
#include "PathfindingStructure.generated.h"

USTRUCT(BlueprintType)
struct FDijkstraNode
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = Node)
		int32 x;
	UPROPERTY(VisibleAnywhere, Category = Node)
		int32 y;
	UPROPERTY(VisibleAnywhere, Category = Node)
		bool bWasProcessed;
	UPROPERTY(VisibleAnywhere, Category = Node)
		bool bIsValidTerrain;
	UPROPERTY(VisibleAnywhere, Category = Node)
		float NodeValue;
};