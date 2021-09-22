// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Public/CharacterStruct.h"
#include "CampaignToolGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API ACampaignToolGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACampaignToolGameModeBase();
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		TSubclassOf<class AGrid> Grid;
	class AGrid* Gridptr;

	//Character Blueprints
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		TSubclassOf<class APlayerCharacter> FighterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		TSubclassOf<class APlayerCharacter> RogueClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		TSubclassOf<class APlayerCharacter> WizardClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Init)
		class APlayerController* UserController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Characters)
		TArray<class ABaseCharacter*> Characters;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		int32 MapChoice;
protected:
	void InitializeGrid();
	void InitializeCharacters();
	void SpawnCharacter(FCharacterStruct character, int32 x, int32 y);
protected:
	FCharacterStruct TestFighter;
	FCharacterStruct TestRogue;
};
