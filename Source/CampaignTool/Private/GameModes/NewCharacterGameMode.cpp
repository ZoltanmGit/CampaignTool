// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/NewCharacterGameMode.h"

ANewCharacterGameMode::ANewCharacterGameMode()
{
	NewCharacter.Level = 1;
	NewCharacter.ArmorClass = 10;

	NewCharacter.Race = ERace::UndefinedRace;
	NewCharacter.Class = EClass::UndefinedClass;
	NewCharacter.CreatureType = ECreatureType::THumanoid;
	NewCharacter.bIsPlayerCharacter = true;
	NewCharacter.Alignment = EAlignment::N;
	AbilityBonusArray.Init(0, 6);
}
void ANewCharacterGameMode::OnElfChoice()
{
	//General
	NewCharacter.Race = ERace::Elf;
	NewCharacter.Speed = 30.0f;
	NewCharacter.Size = ESize::Medium;
	AbilityBonusArray[1] = 2; // +2 to Dexterity
	AbilityBonusArray[3] = 1; // +1 to Intelligence
	//Proficiencies

	//Proficiency-Weapons
	NewCharacter.ProficiencyWeaponArray.Add(EWeapon::Longswords); //Because of "Elf Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EWeapon::Shortswords); //Because of "Elf Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EWeapon::Shortbows); //Because of "Elf Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EWeapon::Longbows); //Because of "Elf Weapon Training"
	//Proficiency-Skills
	NewCharacter.SkillProficiencyArray.Add(ESkill::Perception); //Because of "Keen Senses" feat
	//Proficiency-Saving Throws
	NewCharacter.ConditionSavingThrowArray.Add(ECondition::Charmed); //Because of "Fey Ancestry"
	NewCharacter.ConditionImmunities.Add(ECondition::Sleeping);   //Because of "Fey Ancestry"

	//Feats
	NewCharacter.FeatArray.Add(EFeat::Darkvision);
	NewCharacter.FeatArray.Add(EFeat::FeyAncestry);
	NewCharacter.FeatArray.Add(EFeat::KeenSenses);
	NewCharacter.FeatArray.Add(EFeat::Trance); // Roleplaying purpose only, no in-game effect as of yet

	//Language
	NewCharacter.LanguageArray.Add(ELanguage::Common);
	NewCharacter.LanguageArray.Add(ELanguage::Elvish);
}
void ANewCharacterGameMode::OnDragonbornChoice()
{
	//General
	NewCharacter.Race = ERace::Dragonborn;
	NewCharacter.Speed = 30.0f;
	NewCharacter.Size = ESize::Medium;
	AbilityBonusArray[0] = 2; //+2 to Strength
	AbilityBonusArray[5] = 1; //+1 to Charisma
	NewCharacter.LanguageArray.Add(ELanguage::Common);
	//ADD BREATH WEAPON
}
void ANewCharacterGameMode::OnDwarfChoice()
{
	NewCharacter.Race = ERace::Dwarf;
	NewCharacter.Speed = 25.0f; // TODO: Not Reduced by Heavy Armor 
	NewCharacter.Size = ESize::Medium;
	AbilityBonusArray[2] = 2; // +2 to Constitution
	AbilityBonusArray[4] = 1; // +1 to Wisdom
	//Proficiency-Weapons
	NewCharacter.ProficiencyWeaponArray.Add(EWeapon::Warhammer);    //Because of "Dwarven Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EWeapon::Battleaxe);    //Because of "Dwarven Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EWeapon::LightHammer);  //Because of "Dwarven Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EWeapon::Handaxe);      //Because of "Dwarven Weapon Training"
	//Proficiency-Saving Throws
	NewCharacter.ConditionSavingThrowArray.Add(ECondition::Poisoned);         //Becasue of "Dwarven Resilience"
	NewCharacter.DamageResistanceArray.Add(EDamageType::Poison);              //Becasue of "Dwarven Resilience"
	//Feats
	NewCharacter.FeatArray.Add(EFeat::Darkvision);
	NewCharacter.FeatArray.Add(EFeat::DwarvenResilience);
	NewCharacter.FeatArray.Add(EFeat::DwarvenToughness);
	//Language
	NewCharacter.LanguageArray.Add(ELanguage::Common);
	NewCharacter.LanguageArray.Add(ELanguage::Dwarvish);
}
void ANewCharacterGameMode::OnTieflingChoice()
{
	NewCharacter.Race = ERace::Tiefling;
	NewCharacter.Speed = 30.0f;
	NewCharacter.Size = ESize::Medium;
	AbilityBonusArray[3] = 1; // +1 to INT
	AbilityBonusArray[5] = 2; // +2 to CHA
	//Resistance
	NewCharacter.DamageResistanceArray.Add(EDamageType::Fire); //Because of "Hellish Resistance"
	//Feats
	NewCharacter.FeatArray.Add(EFeat::Darkvision);
	NewCharacter.FeatArray.Add(EFeat::InfernalLegacy);
	NewCharacter.FeatArray.Add(EFeat::HellishResistance);
	//Language
	NewCharacter.LanguageArray.Add(ELanguage::Common);
	NewCharacter.LanguageArray.Add(ELanguage::Infernal);
}

void ANewCharacterGameMode::ResetRaceChoice()
{
	//Reset race
	NewCharacter.Race = ERace::UndefinedRace;
	//Reset the Ability bonuses
	AbilityBonusArray.Empty();
	AbilityBonusArray.Init(0, 6);
	// We empty the arrays
	NewCharacter.FeatArray.Empty();
	NewCharacter.LanguageArray.Empty();
	NewCharacter.SkillProficiencyArray.Empty();
	NewCharacter.ConditionImmunities.Empty();
	NewCharacter.AbilitySavingThrowArray.Empty();
	NewCharacter.ConditionSavingThrowArray.Empty();
	NewCharacter.ProficiencyWeaponArray.Empty();
	NewCharacter.ProficiencyArmorArray.Empty();
	NewCharacter.ProficiencyToolArray.Empty();
}

void ANewCharacterGameMode::OnFighterChoice()
{
	NewCharacter.HitDie = 10;
	NewCharacter.PerLevelHitDie = 6;
	//Armor: All armor, shields
	AddArmorProficiency(EArmor::LightArmor);
	AddArmorProficiency(EArmor::MediumArmor);
	AddArmorProficiency(EArmor::HeavyArmor);
	//Weapons: Simple Weapons, Martial Weapons
	AddWeaponProficiency(EWeapon::SimpleWeapons);
	AddWeaponProficiency(EWeapon::MartialWeapons);
	AddWeaponProficiency(EWeapon::Shield);
	//Tools: None
	//Saving Throws: Strength, Constitution
	AddAbilitySavingThrowProficiency(EAbilityType::Strength);
	AddAbilitySavingThrowProficiency(EAbilityType::Constitution);
	//Skills: Choose 2 from Acrobatics, Animal Handling, Athletics, History, Insight, Intimidation, Perception, and Survival
	//Choose fighting style: Archery, Defense, Dueling, Great Weapon Fighting, Protection, Two Weapon Fighting 
	//Add Second Wind to spells
}

void ANewCharacterGameMode::OnRogueChoice()
{
	NewCharacter.HitDie = 8;
	NewCharacter.PerLevelHitDie = 5;
	//Armor: Light Armor
	AddArmorProficiency(EArmor::LightArmor);
	//Weapons: Simple weapons, hand crossbows, longswords, rapiers, shortswords
	AddWeaponProficiency(EWeapon::Shortswords);
	AddWeaponProficiency(EWeapon::HandCrossbows);
	AddWeaponProficiency(EWeapon::Longswords);
	AddWeaponProficiency(EWeapon::Rapiers);
	AddWeaponProficiency(EWeapon::SimpleWeapons);
	//Tools: Thieves' tools
	AddToolProficiency(ETool::ThievesTools);
	//Saving Throws: Dexterity, Intelligence
	AddAbilitySavingThrowProficiency(EAbilityType::Dexterity);
	AddAbilitySavingThrowProficiency(EAbilityType::Intelligence);
	// Skills: Choose 4 from Acrobatics, Athletics, Deception, Insight, Intimidation, Investigation, Perception, Performance, Persuasion, Sleight of Hand, and Stealth
}

void ANewCharacterGameMode::OnWizardChoice()
{
	NewCharacter.SpellcastingAbility = EAbilityType::Intelligence;
	NewCharacter.SpellSaveDC = 8;
	//Choose 3 Cantrips
	//Choose 6 spells
}

void ANewCharacterGameMode::AddLanguage(const TEnumAsByte<ELanguage> LanguageToAdd)
{
	NewCharacter.LanguageArray.Add(LanguageToAdd);
}

void ANewCharacterGameMode::AddToolProficiency(const TEnumAsByte<ETool> ProficiencyToAdd)
{
	NewCharacter.ProficiencyToolArray.AddUnique(ProficiencyToAdd);
}

void ANewCharacterGameMode::AddWeaponProficiency(const TEnumAsByte<EWeapon> WeaponProficiencyToAdd)
{
	NewCharacter.ProficiencyWeaponArray.AddUnique(WeaponProficiencyToAdd);
}

void ANewCharacterGameMode::AddArmorProficiency(const TEnumAsByte<EArmor> ArmorProficiencyToAdd)
{
	NewCharacter.ProficiencyArmorArray.AddUnique(ArmorProficiencyToAdd);
}

void ANewCharacterGameMode::AddSkillProficiency(const TEnumAsByte<ESkill> SkillProficiencyToAdd)
{
	NewCharacter.SkillProficiencyArray.AddUnique(SkillProficiencyToAdd);
}

void ANewCharacterGameMode::AddResistanceProficiency(const TEnumAsByte<EDamageType> ResistanceProficiencyToAdd)
{
	NewCharacter.DamageResistanceArray.AddUnique(ResistanceProficiencyToAdd);
}

void ANewCharacterGameMode::AddAbilitySavingThrowProficiency(const TEnumAsByte<EAbilityType> AbilityTypeToAdd)
{
	NewCharacter.SavingThrowProficiencyArray.AddUnique(AbilityTypeToAdd);
}
