// Fill out your copyright notice in the Description page of Project Settings.


#include "NewCharacterGameMode.h"

ANewCharacterGameMode::ANewCharacterGameMode()
{
	NewCharacter.Level = 1;
	NewCharacter.Race = Race::UndefinedRace;
	NewCharacter.Class = Class::UndefinedClass;
	NewCharacter.Alignment = Alignment::N;
	AbilityBonusArray.Init(0, 6);
}
void ANewCharacterGameMode::OnElfChoice()
{
	//General
	NewCharacter.Race = Race::Elf;
	NewCharacter.Speed = 30.0f;
	NewCharacter.Size = Size::Medium;
	AbilityBonusArray[1] = 2; // +2 to Dexterity
	//Proficiencies

	//Proficiency-Weapons
	NewCharacter.ProficiencyWeaponArray.Add(Proficiency::Longswords);
	NewCharacter.ProficiencyWeaponArray.Add(Proficiency::Shortswords);
	NewCharacter.ProficiencyWeaponArray.Add(Proficiency::Shortbows);
	NewCharacter.ProficiencyWeaponArray.Add(Proficiency::Longbows);
	//Proficiency-Skills
	NewCharacter.ProficiencySkillArray.Add(Proficiency::Perception); //Because of "Keen Senses" feat
	//Proficiency-Saving Throws
	NewCharacter.ProficiencySavingThrowArray.Add(Proficiency::SavingCharmed); //Because of "Fey Ancestry"
	NewCharacter.ProficiencySavingThrowArray.Add(Proficiency::SavingSleep);   //Because of "Fey Ancestry"

	//Feats
	NewCharacter.FeatArray.Add(Feat::Darkvision);
	NewCharacter.FeatArray.Add(Feat::FeyAncestry);
	NewCharacter.FeatArray.Add(Feat::KeenSenses);
	NewCharacter.FeatArray.Add(Feat::Trance); // Roleplaying purpose only, no in-game effect as of yet

	//Language
	NewCharacter.LanguageArray.Add(Language::Common);
	NewCharacter.LanguageArray.Add(Language::Elvish);
}
void ANewCharacterGameMode::OnHumanChoice()
{
	//General
	NewCharacter.Race = Race::Human;
	NewCharacter.Speed = 30.0f;
	NewCharacter.Size = Size::Medium;
	AbilityBonusArray.Empty(); 
	AbilityBonusArray.Init(6,1); // +1 to all Abilities
	NewCharacter.LanguageArray.Add(Language::Common);
}
void ANewCharacterGameMode::OnDwarfChoice()
{
	NewCharacter.Race = Race::Dwarf;
	NewCharacter.Speed = 25.0f; // TODO: Not Reduced by Heavy Armor 
	NewCharacter.Size = Size::Medium;
	AbilityBonusArray[2] = 2; // +2 to Constitution
	//Proficiency-Weapons
	NewCharacter.ProficiencyWeaponArray.Add(Proficiency::Warhammer); //Because of "Dwarven Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(Proficiency::Battleaxe); //Because of "Dwarven Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(Proficiency::LightHammer); //Because of "Dwarven Weapon Training"
	NewCharacter.ProficiencyWeaponArray.Add(Proficiency::Handaxe); //Because of "Dwarven Weapon Training"
	//Proficiency-Saving Throws
	NewCharacter.ProficiencySavingThrowArray.Add(Proficiency::SavingPoison); //Becasue of "Dwarven Resilience"
	//Resistance
	NewCharacter.ProficiencyResistanceArray.Add(Resistance::PoisonResistance); //Because of "Dwarven Resilience"
	//Feats
	NewCharacter.FeatArray.Add(Feat::Darkvision);
	NewCharacter.FeatArray.Add(Feat::DwarvenResilience);
	NewCharacter.FeatArray.Add(Feat::DwarvenToughness);
	//Language
	NewCharacter.LanguageArray.Add(Language::Common);
	NewCharacter.LanguageArray.Add(Language::Dwarvish);

}
void ANewCharacterGameMode::OnTieflingChoice()
{
	NewCharacter.Race = Race::Tiefling;
	NewCharacter.Speed = 30.0f;
	NewCharacter.Size = Size::Medium;
	AbilityBonusArray[3] = 1; // +1 to INT
	AbilityBonusArray[5] = 2; // +2 to CHA
	//Resistance
	NewCharacter.ProficiencyResistanceArray.Add(Resistance::FireResistance); //Because of "Hellish Resistance"
	//Feats
	NewCharacter.FeatArray.Add(Feat::Darkvision);
	NewCharacter.FeatArray.Add(Feat::InfernalLegacy);
	NewCharacter.FeatArray.Add(Feat::HellishResistance);
	//Language
	NewCharacter.LanguageArray.Add(Language::Common);
	NewCharacter.LanguageArray.Add(Language::Infernal);

}