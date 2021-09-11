// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NewMapMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API ANewMapMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	ANewMapMenuGameMode();

	virtual void BeginPlay() override;
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Map)
		TArray<int32> TileMatrix;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Map)
		TArray<int32> ObjectMatrix;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Map)
		int32 Rows;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Map)
		int32 Columns;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Map)
		int32 SelectedTile;
	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = Map)
		int32 SelectedObject;

	UFUNCTION(BlueprintCallable)
		void ResizeRow(int32 argNewRow);
	UFUNCTION(BlueprintCallable)
		void ResizeColumn(int32 argNewColumn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnGridChange();

};
