// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Character/CharacterSaveObject.h"
#include "NewEncounterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API ANewEncounterGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	ANewEncounterGameMode();

	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Map)
		TArray<int32> TileMatrix;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Map)
		int32 Rows;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Map)
		int32 Columns;
	// Indecies are keys with saveslot strings being values because 
	// A character can only occupy one index
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Characters)
		TMap<int32, FString> CharacterToIndex; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Characters)
		TMap<int32, TSubclassOf<class AAiCharacter>> EnemySpawnMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MetaData)
		TArray<TSubclassOf<class AAiCharacter>> Enemies;
};
