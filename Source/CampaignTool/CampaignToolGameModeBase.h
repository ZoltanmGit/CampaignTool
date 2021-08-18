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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		TSubclassOf<class AGrid> Grid;

	//Character Blueprints
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		TSubclassOf<class APlayerCharacter> FighterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		TSubclassOf<class APlayerCharacter> RogueClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		TSubclassOf<class APlayerCharacter> WizardClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Init)
		class APlayerController* UserController;
protected:
	void InitializeGrid();
	void InitializeCharacters();
	void InitializeMovementController();
};
