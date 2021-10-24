// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathfindingStructure.h"
#include "BaseAbility.h"
#include "AbilityComponent.generated.h"

UENUM(BlueprintType)
enum ETileDirection
{
	D_Up UMETA(DisplayName = "Up"),
	D_Down UMETA(DisplayName = "Down"),
	D_Left UMETA(DisplayName = "Left"),
	D_Right UMETA(DisplayName = "Right"),
	D_UpRight UMETA(DisplayName = "UpRight"),
	D_UpLeft UMETA(DisplayName = "UpLeft"),
	D_DownRight UMETA(DisplayName = "DownRight"),
	D_DownLeft UMETA(DisplayName = "DownLeft")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMPAIGNTOOL_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	/** Properties **/
	UPROPERTY()
		class ABaseCharacter* Owner;

	UPROPERTY(VisibleAnywhere, Category = General)
		class UBaseAbility* SelectedAbility;
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		TArray<class ABaseCharacter*> AffectedCharacters;
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		TArray<int32> AffectedTiles;

	UFUNCTION(BlueprintCallable)
		void HandleTileChange(int32 x, int32 y, float range);
	UFUNCTION(BlueprintCallable)
		void ExecuteAbility();
	UFUNCTION(BlueprintCallable)
		void OnAbilitySelection(class UBaseAbility* AbilityToSelect);
	UFUNCTION(BlueprintCallable)
		void OnAbilityDeselection();

	/** Pathfinding **/
	TQueue<TPair<int32, int32>> DijkstraQueue;
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		int32 Rows;
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		int32 Columns;
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		TArray<FDijkstraNode> DijkstraGrid;
	
	
	/** Bresenham's line algorithm **/

private:
	UFUNCTION(BlueprintCallable)
		void ProcessTile();
	UFUNCTION()
		void GetAffectedCharactersInLine(int32 x0, int32 y0, int32 x1, int32 y1);
};
