// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EncounterSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API UEncounterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	TMap<FString, int32> CharacterSlotToIndexMap;
	FString MapSlotName;
};
