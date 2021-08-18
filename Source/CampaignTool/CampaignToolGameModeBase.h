// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CampaignToolGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API ACampaignToolGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		TSubclassOf<class AMovementController> MovementController;
protected:
	void InitializeGrid();
	void InitializeCharacters();
	void InitializeMovementController();
};
