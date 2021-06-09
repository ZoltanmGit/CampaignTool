// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM(BlueprintType)
enum TileState
{
	Occupied,
	Empty,
	Object
};
UCLASS(BlueprintType,DefaultToInstanced)
class CAMPAIGNTOOL_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileProperties)
		TEnumAsByte<TileState> TileState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TileProperties)
		int X;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TileProperties)
		int Y;
};
