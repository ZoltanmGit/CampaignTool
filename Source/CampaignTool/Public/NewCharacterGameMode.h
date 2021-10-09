// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CharacterStruct.h"
#include "NewCharacterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API ANewCharacterGameMode : public AGameModeBase
{
	GENERATED_BODY()

		ANewCharacterGameMode();
public:


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
		TArray<int32> AbilityBonusArray; // 0-STR || 1-DEX || 2-CON || 3-INT || 4-WIS || 5-CHA
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
		FCharacterStruct NewCharacter;


	UFUNCTION(BlueprintCallable)
		void OnElfChoice();
	UFUNCTION(BlueprintCallable)
		void OnDragonbornChoice();
	UFUNCTION(BlueprintCallable)
		void OnDwarfChoice();
	UFUNCTION(BlueprintCallable)
		void OnTieflingChoice();


	UFUNCTION(BlueprintCallable)
		void ResetRaceChoice();
	UFUNCTION(BlueprintCallable)
		void AddLanguage(const TEnumAsByte<ELanguage> LanguageToAdd);
	UFUNCTION(BlueprintCallable)
		void AddProficiencyToTools(const TEnumAsByte<EProficiency> ProficiencyToAdd);
	UFUNCTION(BlueprintCallable)
		void AddProficiencyToWeapon(const TEnumAsByte<EProficiency> ProficiencyToAdd);
	UFUNCTION(BlueprintCallable)
		void AddProficiencyToArmor(const TEnumAsByte<EProficiency> ProficiencyToAdd);
	UFUNCTION(BlueprintCallable)
		void AddProficiencyToSkills(const TEnumAsByte<EProficiency> ProficiencyToAdd);
	UFUNCTION(BlueprintCallable)
		void AddProficiencyToResistance(const TEnumAsByte<EResistance> ProficiencyToAdd);
	/*UFUNCTION(BlueprintCallabe)
		void AddSpellId();*/ // TODO
};
