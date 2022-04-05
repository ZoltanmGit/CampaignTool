#pragma once

#include "CoreMinimal.h"
#include "Utilities/AttributeEnums.h"
#include "CharacterStruct.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStruct
{
	GENERATED_BODY()

public:
	//General
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		FString CharacterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		TEnumAsByte<ESize> Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		TEnumAsByte<ECreatureType> CreatureType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		int32 ArmorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		int32 ProficiencyBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		int32 Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		int32 HitDie;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		int32 PerLevelHitDie;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		TEnumAsByte<EClass> Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		bool bIsPlayerCharacter;

	//Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Strength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Dexterity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Intelligence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Constitution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Wisdom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Charisma;

	//Background
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Background)
		TEnumAsByte<ERace> Race;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Background)
		TMap<TEnumAsByte<EClass> ,int32> ClassLevelMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Background)
		TEnumAsByte<EAlignment> Alignment;
	

	/** Armor **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EArmor>> ProficiencyArmorArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EWeapon>> ProficiencyWeaponArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<ETool>> ProficiencyToolArray;

	/** Combat Related **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EDamageType>> DamageImmunities; //Damage types to which this character is immune to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<ECondition>> ConditionImmunities; //Condition to which this character is immune to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EAbilityType>> SavingThrowProficiencyArray;

	/** Spellcasting **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spellcasting)
		TEnumAsByte<EAbilityType> SpellcastingAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spellcasting)
		int32 SpellSaveDC;
	/** Saving Throws **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EAbilityType>> AbilitySavingThrowArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<ECondition>> ConditionSavingThrowArray;
	//Damage Resistance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EDamageType>> DamageResistanceArray;
	//Feats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EFeat>> FeatArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<ELanguage>> LanguageArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		TMap<TEnumAsByte<ESkill>, bool> SkillMap;
	// Abilities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellCasting)
		TMap<FString, bool> SpellBook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellCasting)
		TMap<FString, TEnumAsByte<EAbilityType>> AcquiredSpells;
	// Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		TMap<FString, int32> Inventory;
};