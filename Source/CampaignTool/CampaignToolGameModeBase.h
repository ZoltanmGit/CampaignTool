// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Character/CharacterStruct.h"
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
	
	/** IndicatorActor **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Indicator)
		TSubclassOf<class AIndicatorActor> Indicator;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Indicator)
		class AIndicatorActor* Indicatorptr;

	/** Grid **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Grid)
		TSubclassOf<class AGrid> Grid; // type of grid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid)
		class AGrid* Gridptr; // actual reference to grid

	/** Ability Storage **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbilityStorage)
		TSubclassOf<class AAbilityStorage> AbilityStorage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilityStorage)
		class AAbilityStorage* AbilityStorageptr;

	/** Characters **/
	//Assigned direct reference in BP
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Characters)
		TSubclassOf<class APlayerCharacter> FighterClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Characters)
		TSubclassOf<class APlayerCharacter> RogueClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Characters)
		TSubclassOf<class APlayerCharacter> WizardClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Characters)
		TArray<class ABaseCharacter*> Characters;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controller)
		class APlayerController* UserController;
	
	


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		int32 MapChoice;
protected:
	void InitializeGrid();
	void InitializeIndicator();
	void InitializeAbilityStorage();
	void InitializeCharacters();
	void SpawnCharacter(FCharacterStruct character, int32 x, int32 y);
protected:
	FCharacterStruct TestFighter; // Redundant after character creation is implemented
	FCharacterStruct TestRogue;   // Redundant after character creation is implemented
};
