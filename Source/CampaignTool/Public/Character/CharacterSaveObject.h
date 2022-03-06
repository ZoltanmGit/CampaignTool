// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CharacterStruct.h"
#include "CharacterSaveObject.generated.h"

/**
 * 
 */

UCLASS()
class CAMPAIGNTOOL_API UCharacterSaveObject : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Persistence)
		FCharacterStruct SavedCharacterStruct;
};
