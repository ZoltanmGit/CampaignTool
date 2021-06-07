// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"

UENUM(BlueprintType)
enum Proficiency
{
	LightArmor UMETA(DisplayName = "Light Armor"),
	MediumArmor UMETA(DisplayName = "Medium Armor"),
	HeavyArmor UMETA(DisplayName = "Heavy Armor"),
	AllArmor UMETA(DisplayName = "All Armor"),
	Shields UMETA(DisplayName = "Shields"),
	SimpleWeapons UMETA(DisplayName = "Simple Weapons"),
	MartialWeapons UMETA(DisplayName = "Martial Weapons"),
	Clubs UMETA(DisplayName = "Clubs"),
	Daggers UMETA(DisplayName = "Daggers"),
	Darts UMETA(DisplayName = "Darts"),
	Javelins UMETA(DisplayName = "Javelins"),
	Maces UMETA(DisplayName = "Maces"),
	Quarterstaffs UMETA(DisplayName = "Quarterstaffs"),
	Scimitars UMETA(DisplayName = "Scimitars"),
	Sickles UMETA(DisplayName = "Sickles"),
	Slings UMETA(DisplayName = "Slings"),
	Spears UMETA(DisplayName = "Spears"),
	HandCrossbows UMETA(DisplayName = "Hand Crossbows"),
	Longswords UMETA(DisplayName = "Longswords"),
	Shortswords UMETA(DisplayName = "Shortswords"),
	Crossbows UMETA(DisplayName = "Crossbows"),
	LightCrossbows UMETA(DisplayName = "Light Crossbows"),
	Rapiers UMETA(DisplayName = "Rapiers"),
	ThievesTools UMETA(DisplayName = "Thieves' Tools"),
	Athletics UMETA(DisplayName = "Athletics"),
	Acrobatics UMETA(DisplayName = "Acrobatics"),
	SleightOfHand UMETA(DisplayName = "Sleight Of Hand"),
	Stealth UMETA(DisplayName = "Stealth"),
	Arcana UMETA(DisplayName = "Arcana"),
	History UMETA(DisplayName = "History"),
	Investigation UMETA(DisplayName = "Investigation"),
	Nature UMETA(DisplayName = "Nature"),
	Religion UMETA(DisplayName = "Religion"),
	AnimalHandling UMETA(DisplayName = "AnimalHandling"),
	Insight UMETA(DisplayName = "Insight"),
	Medicine UMETA(DisplayName = "Medicine"),
	Perception UMETA(DisplayName = "Perception"),
	Survival UMETA(DisplayName = "Survival"),
	Deception UMETA(DisplayName = "Deception"),
	Intimidation UMETA(DisplayName = "Intimidation"),
	Performance UMETA(DisplayName = "Performance"),
	Persuasion UMETA(DisplayName = "Persuasion"),
	Blindsight UMETA(DisplayName = "BlindSight"),
	Darkvision UMETA(DisplayName = "Darkvision"),
	Tremorsense UMETA(DisplayName = "Tremorsense"),
	Truesight UMETA(DisplayName = "Truesight")
};

UENUM(BlueprintType)
enum Size
{
	Tiny UMETA(DisplayName = "Tiny"),
	Small UMETA(DisplayName = "Small"),
	Medium UMETA(DisplayName = "Medium"),
	Large UMETA(DisplayName = "Large"),
	Huge UMETA(DisplayName = "Huge"),
	Gargantuan UMETA(DisplayName = "Gargantuan"),
};

UENUM(BlueprintType)
enum Race
{
	Dragonborn UMETA(DisplayName = "Dragonborn"),
	Dwarf UMETA(DisplayName = "Dwarf"),
	Elf UMETA(DisplayName = "Elf"),
	Gnome UMETA(DisplayName = "Gnome"),
	HalfElf UMETA(DisplayName = "Half-Elf"),
	Halfling UMETA(DisplayName = "Halfling"),
	HalfOrc UMETA(DisplayName = "Half-Orc"),
	Human UMETA(DisplayName = "Human"),
	Tiefling UMETA(DisplayName = "Tiefling")
};

UENUM(BlueprintType)
enum Class
{
	Barbarian UMETA(DisplayName = "Barbarian"),
	Bard UMETA(DisplayName = "Bard"),
	Cleric UMETA(DisplayName = "Cleric"),
	Druid UMETA(DisplayName = "Druid"),
	Fighter UMETA(DisplayName = "Fighter"),
	Monk UMETA(DisplayName = "Monk"),
	Paladin UMETA(DisplayName = "Paladin"),
	Ranger UMETA(DisplayName = "Ranger"),
	Rogue UMETA(DisplayName = "Rogue"),
	Sorcerer UMETA(DisplayName = "Sorcerer"),
	Warlock UMETA(DisplayName = "Warlock"),
	Wizard UMETA(DisplayName = "Wizard")
};

UENUM(BlueprintType)
enum Language
{
	Telepathy UMETA(DisplayName = "Telepathy"),
	Common UMETA(DisplayName = "Common"),
	Dwarvish UMETA(DisplayName = "Dwarvish"),
	Elvish UMETA(DisplayName = "Elvish"),
	Giant UMETA(DisplayName = "Giant"),
	Gnomish UMETA(DisplayName = "Gnomish"),
	Goblin UMETA(DisplayName = "Goblin"),
	LHalfling UMETA(DisplayName = "Halfling"),
	Orc UMETA(DisplayName = "Orc"),
	Abyssal UMETA(DisplayName = "Abyssal"),
	Celestial UMETA(DisplayName = "Celestial"),
	Draconic UMETA(DisplayName = "Draconic"),
	DeepSpeech UMETA(DisplayName = "Deep Speech"),
	Infernal UMETA(DisplayName = "Infernal"),
	Primordial UMETA(DisplayName = "Primordial"),
	Sylvan UMETA(DisplayName = "Sylvan"),
	Undercommon UMETA(DisplayName = "Undercommon")
};

UENUM(BlueprintType)
enum Alignment
{
	LG UMETA(DisplayName = "Lawful Good"),
	NG UMETA(DisplayName = "Neutral Good"),
	CG UMETA(DisplayName = "Chaotic Good"),
	LN UMETA(DisplayName = "Lawful Neutral"),
	N UMETA(DisplayName = "Neutral"),
	CN UMETA(DisplayName = "Chaotic Neutral"),
	LE UMETA(DisplayName = "Lawful Evil"),
	NE UMETA(DisplayName = "Neutral Evil"),
	CE UMETA(DisplayName = "Chaotic Evil"),
	Unaligned UMETA(DisplayName = "Unaligned")
};

UENUM(BlueprintType)
enum Faction
{
	F_Unaligned UMETA(DisplayName = "Unaligned")
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMPAIGNTOOL_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//General
	int level;
	int proeficiencyBonus;
	
	//Character Attributes
	int strength;
	int dexterity;
	int intelligence;
	int constitution;
	int wisdom;
	int charisma;
	int speed;

	//Character Race and Background
	
	//Skills - Strength
	int athletics;
	//Skills - Dexterity
	int acrobatics;
	int sleightOfHand;
	int stealth;
	//Skills - Intelligence
	int arcana;
	int history;
	int investigation;
	int nature;
	int religion;
	//Skills - Wisdom
	int animalHandling;
	int insight;
	int medicine;
	int perception;
	int survival;
	//Skills - Charisma
	int deception;
	int intimidation;
	int performance;
	int persuasion;

public:
	UFUNCTION(BlueprintCallable)
		int GetModifier(int value);
	UFUNCTION(BlueprintCallable)
		int GetStrength(int value);
	UFUNCTION(BlueprintCallable)
		int GetDexterity(int value);
	UFUNCTION(BlueprintCallable)
		int GetIntelligence(int value);
	UFUNCTION(BlueprintCallable)
		int GetWisdom(int value);
	UFUNCTION(BlueprintCallable)
		int GetConstitution(int value);
	UFUNCTION(BlueprintCallable)
		int GetCharisma(int value);

	UFUNCTION(BlueprintCallable)
		int GetAthletics(int value);

	UFUNCTION(BlueprintCallable)
		int GetAcrobatics(int value);
	UFUNCTION(BlueprintCallable)
		int GetSleightOfHand(int value);
	UFUNCTION(BlueprintCallable)
		int GetStealth(int value);

	UFUNCTION(BlueprintCallable)
		int GetArcana(int value);
	UFUNCTION(BlueprintCallable)
		int GetHistory(int value);
	UFUNCTION(BlueprintCallable)
		int GetInvestigation(int value);
	UFUNCTION(BlueprintCallable)
		int GetNature(int value);
	UFUNCTION(BlueprintCallable)
		int GetReligion(int value);

	UFUNCTION(BlueprintCallable)
		int GetAnimalHandling(int value);
	UFUNCTION(BlueprintCallable)
		int GetInsight(int value);
	UFUNCTION(BlueprintCallable)
		int GetMedicine(int value);
	UFUNCTION(BlueprintCallable)
		int GetPerception(int value);
	UFUNCTION(BlueprintCallable)
		int GetSurvival(int value);

	UFUNCTION(BlueprintCallable)
		int GetDeception(int value);
	UFUNCTION(BlueprintCallable)
		int GetIntimidation(int value);
	UFUNCTION(BlueprintCallable)
		int GetPerformance(int value);
	UFUNCTION(BlueprintCallable)
		int GetPersuasion(int value);
};
