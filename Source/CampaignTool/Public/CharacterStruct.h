#pragma once

#include "CoreMinimal.h"
#include "AttributeEnums.h"
#include "CharacterStruct.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStruct
{
	GENERATED_BODY()

public:
	//General
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		FString CharacterName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		TEnumAsByte<ESize> Size;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		TEnumAsByte<ECreatureType> CreatureType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		int32 Level;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		int32 ArmorClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		int32 ProficiencyBonus;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		int32 Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		int32 HitDie;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		int32 PerLevelHitDie;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		bool bIsPlayerCharacter;

	//Attributes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Strength;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Dexterity;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Intelligence;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Constitution;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Wisdom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
		int32 Charisma;

	//Background
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Background)
		TEnumAsByte<ERace> Race;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Background)
		TEnumAsByte<EClass> Class;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Background)
		TEnumAsByte<EAlignment> Alignment;
	

	//Equipment
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EArmor>> ProficiencyArmorArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EWeapon>> ProficiencyWeaponArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<ETool>> ProficiencyToolArray;
	//Combat related
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EDamageType>> DamageImmunities; //Damage types to which this character is immune to
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<ECondition>> ConditionImmunities; //Condition to which this character is immune to
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EAbilityType>> SavingThrowProficiencyArray;

	/** Spellcasting **/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Spellcasting)
		TEnumAsByte<EAbilityType> SpellcastingAbility;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Spellcasting)
		int32 SpellSaveDC;
	/** Saving Throws **/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EAbilityType>> AbilitySavingThrowArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<ECondition>> ConditionSavingThrowArray;
	//Damage Resistance
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EDamageType>> DamageResistanceArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<ESkill>> SkillProficiencyArray;
	//Feats
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EFeat>> FeatArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<ELanguage>> LanguageArray;

	//Skills
	//Skills - Strength
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Athletics;
	//Skills - Dexterity
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Acrobatics;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 SleightOfHand;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Stealth;
	//Skills - Intelligence
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Arcana;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 History;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Investigation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Nature;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Religion;
	//Skills - Wisdom
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 AnimalHandling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Insight;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Medicine;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Perception;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Survival;
	//Skills - Charisma
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Deception;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Intimidation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Performance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skills)
		int32 Persuasion;
	// Abilities
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SpellCasting)
		TMap<FString, bool> SpellBook;
	// Inventory
	// TODO
};