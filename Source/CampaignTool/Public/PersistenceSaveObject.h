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
		int32 SavedMapNum;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = SaveData)
		int32 SavedCharacterNum;
};
