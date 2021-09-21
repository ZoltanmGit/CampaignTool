// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MapSaveObject.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API UMapSaveObject : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PersistentData)
		TArray<int32> TileMatrix;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PersistentData)
		int32 Rows;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PersistentData)
		int32 Columns;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PersistentData)
		FName MapName;
};
