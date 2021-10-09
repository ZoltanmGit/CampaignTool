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
		int32 Level;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		int32 ProficiencyBonus;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		int32 Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		int32 HitDie;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = General)
		TEnumAsByte<ESize> Size;

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
	//Proficiencies
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EProficiency>> ProficiencyArmorArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EProficiency>> ProficiencyWeaponArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EProficiency>> ProficiencySkillArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EProficiency>> ProficiencySavingThrowArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EResistance>> ProficiencyResistanceArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Proficiencies)
		TArray<TEnumAsByte<EProficiency>> ProficiencyToolArray;

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
};