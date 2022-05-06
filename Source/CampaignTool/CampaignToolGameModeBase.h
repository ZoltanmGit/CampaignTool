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
		TSubclassOf<class AGrid> Grid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid)
		class AGrid* Gridptr;

	/** Ability Storage **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbilityStorage)
		TSubclassOf<class AAbilityStorage> AbilityStorage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilityStorage)
		class AAbilityStorage* AbilityStorageptr;

	/** ItemStorage **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemStorage)
		TSubclassOf<class AItemStorage> ItemStorage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStorage)
		class AItemStorage* ItemStorageptr;

	/** TemplateClasses **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Characters)
		TSubclassOf<class APlayerCharacter> FighterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Characters)
		TSubclassOf<class APlayerCharacter> RogueClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Characters)
		TSubclassOf<class APlayerCharacter> WizardClass;
	
	/** Character Arrays **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Characters)
		TArray<class ABaseCharacter*> Characters;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Characters)
		TArray<class ABaseCharacter*> PlayerCharacters;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Characters)
		TArray<class ABaseCharacter*> EnemyCharacters;
	
	APlayerCharacter* previousCharacter; //The purpose of this pointer is only tied to cameras being rotated when changing posession for a more seamless transition



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controller)
		class APlayerController* UserController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enemy)
		class AEnemyController* EnemyController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Init)
		int32 MapChoice;

	/** Gameplay Controllers **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
		int32 turnIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
		int32 preparationTurnIndex;
	UFUNCTION(BlueprintCallable)
		void NextTurn();
protected:
		UFUNCTION(BlueprintCallable)
	void InitializeGrid();
		UFUNCTION(BlueprintCallable)
	void InitializeIndicator();
		UFUNCTION(BlueprintCallable)
	void InitializeAbilityStorage();
		UFUNCTION(BlueprintCallable)
	void InitializeItemStorage();
		UFUNCTION(BlueprintCallable)
	void InitializeCharacters();
		UFUNCTION(BlueprintCallable)
	void InitializeEnemies();
		UFUNCTION(BlueprintCallable)
	void SortInitiative();
	

	void SpawnCharacter(FCharacterStruct character, int32 x, int32 y);
};
