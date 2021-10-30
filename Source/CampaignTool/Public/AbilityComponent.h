// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathfindingStructure.h"
#include "BaseAbility.h"
#include "AbilityComponent.generated.h"

UENUM(BlueprintType)
enum EConeDirection
{
	D_Undefined UMETA(DisplayName = "D_Undefined"),
	D_Up UMETA(DisplayName = "Up"),
	D_Down UMETA(DisplayName = "Down"),
	D_Left UMETA(DisplayName = "Left"),
	D_Right UMETA(DisplayName = "Right"),
	D_UpRight UMETA(DisplayName = "UpRight"),
	D_UpLeft UMETA(DisplayName = "UpLeft"),
	D_DownRight UMETA(DisplayName = "DownRight"),
	D_DownLeft UMETA(DisplayName = "DownLeft"),
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
	
	/** Utility **/
	UFUNCTION(BlueprintCallable)
		void HandleTileChange();
	UFUNCTION(BlueprintCallable)
		void ExecuteAbility();
	UFUNCTION(BlueprintCallable)
		void SelectAbility(class UBaseAbility* AbilityToSelect);

	/** Dijkstra's algorithm **/
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		int32 Rows;
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		int32 Columns;
	UPROPERTY(VisibleAnywhere, Category = Pathfinding)
		TArray<FDijkstraNode> DijkstraGrid;
	TQueue<TPair<int32, int32>> DijkstraQueue;
public:
	
	/** Dijkstra's algorithm **/
	
	/** Bresenham's line algorithm **/
	UFUNCTION()
		void ResolveLine(int32 x0, int32 y0, int32 x1, int32 y1);
	
	/** Cone Algorithm **/
	UFUNCTION()
		void ResolveCone(int32 x, int32 y, TEnumAsByte<EConeDirection> Direction);

	
	/** General Functionality **/
	void CancelAbility();
	void ResetPathfinding();
	bool HasSelectedAbility(); // Redundant? 

private:
	
	/** Cone Algorithm **/
	TEnumAsByte<EConeDirection> GetConeDirection(int32 x_char, int32 y_char, int32 x_curs, int32 y_curs);
	void ResolveConeHorizontal(int32 x_char, int32 y_char, TEnumAsByte<EConeDirection> Direction);
	void ResolveConeVertical(int32 x_char, int32 y_char, TEnumAsByte<EConeDirection> Direction);
	void ResolveConeDiagonal(int32 x_char, int32 y_char, TEnumAsByte<EConeDirection> Direction);
	void ProcessNodeForCone(int32 x,int32 y,float range, TEnumAsByte<EConeDirection> Direction );

	UPROPERTY(VisibleAnywhere, Category = "Cone")
		TArray<FDijkstraNode> ConeDijkstraGrid;
	
	TQueue<TPair<int32,int32>> ConeDijkstraQueue;

	
	/** Bresenham's line algorithm **/
	void PlotTileLow(int32 x0, int32 y0, int32 x1, int32 y1, bool bWasSwitched);
	void PlotTileHigh(int32 x0, int32 y0, int32 x1, int32 y1, bool bWasSwitched);
};
