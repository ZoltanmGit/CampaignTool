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
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
		TArray<int32> AbilityBonusArray; // 0-STR || 1-DEX || 2-CON || 3-INT || 4-WIS || 5-CHA
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
		FCharacterStruct NewCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Map)
		FString CharacterName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Persistence)
		class UCharacterSaveObject* SaveObject;

	//Race Choice
	UFUNCTION(BlueprintCallable)
		void OnElfChoice();
	UFUNCTION(BlueprintCallable)
		void OnDragonbornChoice(const FString Breath);
	UFUNCTION(BlueprintCallable)
		void OnDwarfChoice();
	UFUNCTION(BlueprintCallable)
		void OnTieflingChoice();
	UFUNCTION(BlueprintCallable)
		void ResetRaceChoice();
	//Class choice
	UFUNCTION(BlueprintCallable)
		void OnFighterChoice(const FString Skill01, const FString Skill02, const FString FightingStyle);
	UFUNCTION(BlueprintCallable)
		void RevertFighterChoice(const FString Skill01, const FString Skill02, const FString FightingStyle);
	UFUNCTION(BlueprintCallable)
		void OnRogueChoice(const FString Skill01, const FString Skill02);
	UFUNCTION(BlueprintCallable)
		void RevertRogueChoice();
	UFUNCTION(BlueprintCallable)
		void OnWizardChoice(const FString Skill01, const FString Skill02);
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
	UFUNCTION(BlueprintCallable)
		bool IsProficientWith(const TEnumAsByte<ESkill> skill);
	UFUNCTION(BlueprintCallable)
		void FinalizeAbilities(const int32 Str, const int32 Dex, const int32 Con, const int32 Int, const int32 Wis, const int32 Cha);
	UFUNCTION(BlueprintCallable)
		void SaveCharacter();
private:
	void RemoveAbilitySavingThrowProficiency(TEnumAsByte<EAbilityType> ProficiencyToRemvoce);
	void RemoveWeaponProficiency(TEnumAsByte<EWeapon> WeaponProficiencyToRemove);
	void RemoveArmorProficiency(TEnumAsByte<EArmor> ArmorProficiencyToRemove);
	void RemoveSkillProficiency(const TEnumAsByte<ESkill> SkillToRemove);
	TEnumAsByte<ESkill> GetSkillFromString(const FString skillAsString);
	TEnumAsByte<EFeat> GetFightingStyleFromString(const FString fightingStyleAsString);
	void RevertSkillMap();
};
