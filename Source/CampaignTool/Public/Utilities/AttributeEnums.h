#pragma once

#include "CoreMinimal.h"
#include "AttributeEnums.generated.h"

UENUM(BlueprintType)
enum EArmor
{
	Unarmored UMETA(DisplayName = "Unarmored"),
	LightArmor UMETA(DisplayName = "Light Armor"),
	MediumArmor UMETA(DisplayName = "Medium Armor"),
	HeavyArmor UMETA(DisplayName = "Heavy Armor")
};

UENUM(BlueprintType)
enum EWeaponProperty
{
	WP_Finesse UMETA(DisplayName = "Finesse"),
	WP_Heavy UMETA(DisplayName = "Heavy"),
	WP_Light UMETA(DisplayName = "Light"),
	WP_Loading UMETA(DisplayName = "Loading"),
	WP_Thrown UMETA(DisplayName = "Thrown")
};

UENUM(BlueprintType)
enum EWeapon
{
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
	Shortbows UMETA(DisplayName = "Shortbows"),
	Longbows UMETA(DisplayName = "Longbows"),
	Rapiers UMETA(DisplayName = "Rapiers"),
	Battleaxe UMETA(DisplayName = "Battleaxe"),
	Handaxe UMETA(DisplayName = "Handaxe"),
	LightHammer UMETA(DisplayName = "LightHammer"),
	Warhammer UMETA(DisplayName = "Warhammer"),
	Shield UMETA(DisplayName = "Shield")
};

UENUM(BlueprintType)
enum ESkill
{
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
	Persuasion UMETA(DisplayName = "Persuasion")
};

UENUM(BlueprintType)
enum ETool
{
	ThievesTools UMETA(DisplayName = "Thieves' tools"),
	SmithsTools UMETA(DisplayName = "Smith's tools"),
	BrewersSupplies UMETA(DisplayName = "Brewer's supplies"),
	MasonsTools UMETA(DisplayName = "Mason's tools"),
};

UENUM(BlueprintType)
enum ESize
{
	Tiny UMETA(DisplayName = "Tiny"),
	Small UMETA(DisplayName = "Small"),
	Medium UMETA(DisplayName = "Medium"),
	Large UMETA(DisplayName = "Large"),
	Huge UMETA(DisplayName = "Huge"),
	Gargantuan UMETA(DisplayName = "Gargantuan"),
};

UENUM(BlueprintType)
enum ERace
{
	UndefinedRace UMETA(DisplayName = "UndefinedRace"),
	Dragonborn UMETA(DisplayName = "Dragonborn"),
	Dwarf UMETA(DisplayName = "Dwarf"),
	Elf UMETA(DisplayName = "Elf"),
	Tiefling UMETA(DisplayName = "Tiefling")
};

UENUM(BlueprintType)
enum EClass
{
	UndefinedClass UMETA(DisplayName = "UndefinedClass"),
	Fighter UMETA(DisplayName = "Fighter"),
	Rogue UMETA(DisplayName = "Rogue"),
	Wizard UMETA(DisplayName = "Wizard")
};

UENUM(BlueprintType)
enum EFeat
{
	UndefinedFeat UMETA(DisplayName = "UndefinedFeat"),
	Blindsight UMETA(DisplayName = "BlindSight"),
	Darkvision UMETA(DisplayName = "Darkvision"),
	Tremorsense UMETA(DisplayName = "Tremorsense"),
	Truesight UMETA(DisplayName = "Truesight"),
	KeenSenses UMETA(DisplayName = "KeenSenses"),
	FeyAncestry UMETA(DisplayName = "FeyAncestry"),
	Trance UMETA(DisplayName = "Trance"),
	DwarvenCombatTraining UMETA(DisplayName = "DwarvenCombatTraining"),
	DwarvenResilience UMETA(DisplayName = "DwarvenResilience"),
	DwarvenToughness UMETA(DisplayName = "DwarvenToughness"),
	Stonecunning UMETA(DisplayName = "Stonecunning"),
	HellishResistance UMETA(DisplayName = "HellishResistance"),
	InfernalLegacy UMETA(DisplayName = "InfernalLegacy"),
	ElfWeaponTraining UMETA(DisplayName = "ElfWeaponTraning"),
	SneakAttack UMETA(DisplayName = "SneakAttack"),
	TheivesCant UMETA(DisplayName = "Thieves' cant"),
	Expertise UMETA(DisplayName = "Expertise")


};

UENUM(BlueprintType)
enum ELanguage
{
	
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
	Undercommon UMETA(DisplayName = "Undercommon"),

	//Should be special and unused for a while
	Telepathy UMETA(DisplayName = "Telepathy")
};

UENUM(BlueprintType)
enum EAlignment
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
};

UENUM(BlueprintType)
enum EFaction
{
	Unaligned UMETA(DisplayName = "Unaligned")
};

UENUM(BlueprintType)
enum EDamageType
{
	Poison UMETA(DisplayName = "Poison"),
	Fire UMETA(DisplayName = "Fire"),
	Acid UMETA(DisplayName = "Acid"),
	Cold UMETA(DisplayName = "Cold"),
	Lightning UMETA(DisplayName = "Lightning"),
	Bludgeoning UMETA(DisplayName = "Bludgeoning"),
	Slashing UMETA(DisplayName = "Slashing"),
	Piercing UMETA(DisplayName = "Piercing"),
	Force UMETA(DisplayName = "Force"),
	Thunder UMETA(DisplayName = "Thunder"),
	Necrotic UMETA(DisplayName = "Necrotic"),
	Radiant UMETA(DisplayName = "Radiant"),
	Psychic UMETA(DisplayName = "Psychic"),
	Healing UMETA(DisplayName = "Healing")
};

UENUM(BlueprintType)
enum ECondition
{
	Paralyzed UMETA(DisplayName = "Paralyzed"),
	Petrified UMETA(DisplayName = "Petrified"),
	Blinded UMETA(DisplayName = "Blinded"),
	Charmed UMETA(DisplayName = "Charmed"),
	Deafened UMETA(DisplayName = "Deafened"),
	Exhausted UMETA(DisplayName = "Exhausted"),
	Poisoned UMETA(DisplayName = "Poisoned"),
	Frightened UMETA(DisplayName = "Frightened"),
	Grappled UMETA(DisplayName = "Grappled"),
	Incapacitated UMETA(DisplayName = "Incapacitated"),
	Invisible UMETA(DisplayName = "Invisible"),
	Prone UMETA(DisplayName = "Prone"),
	Restrained UMETA(DisplayName = "Restrained"),
	Stunned UMETA(DisplayName = "Stunned"),
	Unconscious UMETA(DisplayName = "Unconscious"),
	Sleeping UMETA(DisplayName = "Sleeping"),
};

UENUM(BlueprintType)
enum EAbilityType
{
	UndefiniedAbility UMETA(DisplayName = "UndefinedAbility"),
	Strength UMETA(DisplayName = "Strength"),
	Dexterity UMETA(DisplayName = "Dexterity"),
	Constitution UMETA(DisplayName = "Constitution"),
	Intelligence UMETA(DisplayName = "Intelligence"),
	Wisdom UMETA(DisplayName = "Wisdom"),
	Charsima UMETA(DisplayName = "Charisma")
};

UENUM(BlueprintType)
enum ECreatureType
{
	TAberration UMETA(DisplayName = "Aberration"),
	TBeast UMETA(DisplayName = "Beast"),
	TCelestial UMETA(DisplayName = "Celestial"),
	TConstruct UMETA(DisplayName = "Construct"),
	TDragon UMETA(DisplayName = "Dragon"),
	TElemental UMETA(DisplayName = "Elemental"),
	TFey UMETA(DisplayName = "Fey"),
	TFiend UMETA(DisplayName = "Fiend"),
	TGiant UMETA(DisplayName = "Giant"),
	THumanoid UMETA(DisplayName = "Humanoid"),
	TMonstrosity UMETA(DisplayName = "Monstrosity"),
	TOoze UMETA(DisplayName = "Ooze"),
	TPlant UMETA(DisplayName = "Plant"),
	TUndead UMETA(DisplayName = "Undead")
};

UENUM(BlueprintType)
enum ECharacterType
{
	C_Ally,
	C_Hostile,
	C_Self
};

UENUM(BlueprintType)
enum EItemType
{
	ItemType_Armor UMETA(DisplayName = "Armor"),
	ItemType_Weapon UMETA(DisplayName = "Weapon"),
	ItemType_Consumable UMETA(DisplayName = "Consumable"),
	ItemType_Accessory UMETA(DisplayName = "Accessory"),
	ItemType_Ammunation UMETA(DisplayName = "Ammunation"),
	ItemType_Shield UMETA(DisplayName = "Shield")
};

UENUM(BlueprintType)
enum EAmmunitionType
{
	Arrow UMETA(DisplayName = "Arrow"),
	Bolt UMETA(DisplayName = "Bolt"),
	Bullet UMETA(DisplayName = "Bullet")
};

UENUM(BlueprintType)
enum ERangeType
{
	Melee UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged"),
};
// Ctrl+C: UMETA(DisplayName = ""),