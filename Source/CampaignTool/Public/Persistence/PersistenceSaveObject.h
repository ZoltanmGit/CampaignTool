// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PersistenceSaveObject.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API UPersistenceSaveObject : public USaveGame
{
	GENERATED_BODY()
public:
	UPersistenceSaveObject();
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = SaveData)
		int32 MapNextIndex;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = SaveData)
		int32 CharacterNextIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterSaveData)
		TArray<FString> CharacterSlotNames;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterSaveData)
		TArray<FString> MapSlotNames;
};
