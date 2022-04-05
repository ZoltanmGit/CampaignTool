// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CampaignToolGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API UCampaignToolGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MetaData)
		FString SelectedMapSlotName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MetaData)
		FString SelectedCharacterSlotName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MetaData)
		TMap<int32, FString> IndexForCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MetaData)
		TMap<int32, class AAiCharacter*> EnemySpawnMap;
};
