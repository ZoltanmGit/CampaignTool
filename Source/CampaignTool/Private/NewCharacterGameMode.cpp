// Fill out your copyright notice in the Description page of Project Settings.


#include "NewCharacterGameMode.h"

ANewCharacterGameMode::ANewCharacterGameMode()
{
	NewCharacter.Level = 1;
	NewCharacter.Race = ERace::UndefinedRace;
	NewCharacter.Class = EClass::UndefinedClass;
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
	NewCharacter.ProficiencyWeaponArray.Add(EProficiency::Longswords); //Because of "Elf Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EProficiency::Shortswords); //Because of "Elf Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EProficiency::Shortbows); //Because of "Elf Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EProficiency::Longbows); //Because of "Elf Weapon Training"
	//Proficiency-Skills
	NewCharacter.ProficiencySkillArray.Add(EProficiency::Perception); //Because of "Keen Senses" feat
	//Proficiency-Saving Throws
	NewCharacter.ProficiencySavingThrowArray.Add(EProficiency::SavingCharmed); //Because of "Fey Ancestry"
	NewCharacter.ProficiencySavingThrowArray.Add(EProficiency::SavingSleep);   //Because of "Fey Ancestry"

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
}
void ANewCharacterGameMode::OnDwarfChoice()
{
	NewCharacter.Race = ERace::Dwarf;
	NewCharacter.Speed = 25.0f; // TODO: Not Reduced by Heavy Armor 
	NewCharacter.Size = ESize::Medium;
	AbilityBonusArray[2] = 2; // +2 to Constitution
	AbilityBonusArray[4] = 1; // +1 to Wisdom
	//Proficiency-Weapons
	NewCharacter.ProficiencyWeaponArray.Add(EProficiency::Warhammer); //Because of "Dwarven Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EProficiency::Battleaxe); //Because of "Dwarven Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EProficiency::LightHammer); //Because of "Dwarven Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(EProficiency::Handaxe); //Because of "Dwarven Weapon Training"
	//Proficiency-Saving Throws
	NewCharacter.ProficiencySavingThrowArray.Add(EProficiency::SavingPoison); //Becasue of "Dwarven Resilience"
	//Resistance
	NewCharacter.ProficiencyResistanceArray.Add(EResistance::PoisonResistance); //Because of "Dwarven Resilience"
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
	NewCharacter.ProficiencyResistanceArray.Add(EResistance::FireResistance); //Because of "Hellish Resistance"
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
	NewCharacter.ProficiencyResistanceArray.Empty();
	NewCharacter.ProficiencySavingThrowArray.Empty();
	NewCharacter.ProficiencyWeaponArray.Empty();
	NewCharacter.ProficiencyToolArray.Empty();
}

void ANewCharacterGameMode::AddLanguage(const TEnumAsByte<ELanguage> LanguageToAdd)
{
	NewCharacter.LanguageArray.Add(LanguageToAdd);
}

void ANewCharacterGameMode::AddProficiencyToTools(const TEnumAsByte<EProficiency> ProficiencyToAdd)
{
	NewCharacter.ProficiencyToolArray.Add(ProficiencyToAdd);
}

void ANewCharacterGameMode::AddProficiencyToWeapon(const TEnumAsByte<EProficiency> WeaponProficiencyToAdd)
{
	NewCharacter.ProficiencyWeaponArray.Add(WeaponProficiencyToAdd);
}

void ANewCharacterGameMode::AddProficiencyToArmor(const TEnumAsByte<EProficiency> ArmorProficiencyToAdd)
{
	NewCharacter.ProficiencyArmorArray.Add(ArmorProficiencyToAdd);
}

void ANewCharacterGameMode::AddProficiencyToSkills(const TEnumAsByte<EProficiency> SkillProficiencyToAdd)
{
	NewCharacter.ProficiencySkillArray.Add(SkillProficiencyToAdd);
}

void ANewCharacterGameMode::AddProficiencyToResistance(const TEnumAsByte<EResistance> ResistanceProficiencyToAdd)
{
	NewCharacter.ProficiencyResistanceArray.Add(ResistanceProficiencyToAdd);
}
