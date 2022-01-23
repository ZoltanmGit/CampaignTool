// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Character/CharacterStruct.h"
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

	//Race Choice
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
	//Class choice
	UFUNCTION(BlueprintCallable)
		void OnFighterChoice();
	UFUNCTION(BlueprintCallable)
		void RevertFighterChoice();
	UFUNCTION(BlueprintCallable)
		void OnRogueChoice();
	UFUNCTION(BlueprintCallable)
		void RevertRogueChoice();
	UFUNCTION(BlueprintCallable)
		void OnWizardChoice();
	UFUNCTION(BlueprintCallable)
		void RevertWizardChoice();

	UFUNCTION(BlueprintCallable)
		void AddLanguage(const TEnumAsByte<ELanguage> LanguageToAdd);
	UFUNCTION(BlueprintCallable)
		void AddToolProficiency(const TEnumAsByte<ETool> ProficiencyToAdd);
	UFUNCTION(BlueprintCallable)
		void AddWeaponProficiency(const TEnumAsByte<EWeapon> ProficiencyToAdd);
	UFUNCTION(BlueprintCallable)
		void AddArmorProficiency(const TEnumAsByte<EArmor> ProficiencyToAdd);
	UFUNCTION(BlueprintCallable)
		void AddSkillProficiency(const TEnumAsByte<ESkill> ProficiencyToAdd);
	UFUNCTION(BlueprintCallable)
		void AddResistanceProficiency(const TEnumAsByte<EDamageType> ProficiencyToAdd);
	UFUNCTION(BlueprintCallable)
		void AddAbilitySavingThrowProficiency(const TEnumAsByte<EAbilityType> ProficiencyToAdd);

	void RemoveAbilitySavingThrowProficiency(TEnumAsByte<EAbilityType> ProficiencyToRemvoce);
	void RemoveWeaponProficiency(TEnumAsByte<EWeapon> WeaponProficiencyToRemove);
	void RemoveArmorProficiency(TEnumAsByte<EArmor>ArmorProficiencyToRemove);

	/*UFUNCTION(BlueprintCallabe)
		void AddSpellId();*/ // TODO
};
