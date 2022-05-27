// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/NewCharacterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Persistence/PersistenceSaveObject.h"
#include "Character/CharacterSaveObject.h"

ANewCharacterGameMode::ANewCharacterGameMode()
{
	NewCharacter.ClassLevelMap.Add(EClass::Fighter, 0);
	NewCharacter.ClassLevelMap.Add(EClass::Rogue, 0);
	NewCharacter.ClassLevelMap.Add(EClass::Wizard, 0);
	NewCharacter.ClassLevelMap.Add(EClass::UndefinedClass, 0);
	NewCharacter.ArmorClass = 10;
	NewCharacter.ProficiencyBonus = 2;

	{
		// Painful init
		NewCharacter.SkillMap.Add(ESkill::Acrobatics, false);
		NewCharacter.SkillMap.Add(ESkill::Athletics, false);
		NewCharacter.SkillMap.Add(ESkill::AnimalHandling, false);
		NewCharacter.SkillMap.Add(ESkill::Arcana, false);
		NewCharacter.SkillMap.Add(ESkill::Deception, false);
		NewCharacter.SkillMap.Add(ESkill::History, false);
		NewCharacter.SkillMap.Add(ESkill::Insight, false);
		NewCharacter.SkillMap.Add(ESkill::Intimidation, false);
		NewCharacter.SkillMap.Add(ESkill::Investigation, false);
		NewCharacter.SkillMap.Add(ESkill::Medicine, false);
		NewCharacter.SkillMap.Add(ESkill::Nature, false);
		NewCharacter.SkillMap.Add(ESkill::Perception, false);
		NewCharacter.SkillMap.Add(ESkill::Performance, false);
		NewCharacter.SkillMap.Add(ESkill::Persuasion, false);
		NewCharacter.SkillMap.Add(ESkill::Religion, false);
		NewCharacter.SkillMap.Add(ESkill::SleightOfHand, false);
		NewCharacter.SkillMap.Add(ESkill::Stealth, false);
		NewCharacter.SkillMap.Add(ESkill::Survival, false);
	}

	NewCharacter.Race = ERace::UndefinedRace;
	NewCharacter.CreatureType = ECreatureType::THumanoid;
	NewCharacter.bIsPlayerCharacter = true;
	NewCharacter.Alignment = EAlignment::N;
	AbilityBonusArray.Init(0, 6);
}

void ANewCharacterGameMode::BeginPlay()
{
	SaveObject = Cast<UCharacterSaveObject>(UGameplayStatics::CreateSaveGameObject(UCharacterSaveObject::StaticClass()));
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
	AddSkillProficiency(ESkill::Perception); //Because of "Keen Senses" feat
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

	NewCharacter.AcquiredSpells.Add("g_elfspell", EAbilityType::UndefiniedAbility);
}

void ANewCharacterGameMode::OnDragonbornChoice(const FString Breath)
{
	//General
	NewCharacter.Race = ERace::Dragonborn;
	NewCharacter.Speed = 30.0f;
	NewCharacter.Size = ESize::Medium;
	AbilityBonusArray[0] = 2; //+2 to Strength
	AbilityBonusArray[5] = 1; //+1 to Charisma
	NewCharacter.LanguageArray.Add(ELanguage::Common);
	//ADD BREATH WEAPON
	if (Breath == "Black")
	{
		NewCharacter.AcquiredSpells.Add("g_acidbreath", EAbilityType::UndefiniedAbility);
	}
	else if (Breath == "Bronze")
	{
		NewCharacter.AcquiredSpells.Add("g_lightningbreath", EAbilityType::UndefiniedAbility);
	}
	else if (Breath == "Gold")
	{
		NewCharacter.AcquiredSpells.Add("g_firebreath", EAbilityType::UndefiniedAbility);
	}
	else if (Breath == "Silver")
	{
		NewCharacter.AcquiredSpells.Add("g_coldbreath", EAbilityType::UndefiniedAbility);
	}
}

void ANewCharacterGameMode::OnDwarfChoice()
{
	NewCharacter.Race = ERace::Dwarf;
	NewCharacter.Speed = 25.0f; 
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

	NewCharacter.AcquiredSpells.Add("g_tieflingspell", EAbilityType::UndefiniedAbility);
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
	NewCharacter.ConditionImmunities.Empty();
	NewCharacter.AbilitySavingThrowArray.Empty();
	NewCharacter.ConditionSavingThrowArray.Empty();
	NewCharacter.ProficiencyWeaponArray.Empty();
	NewCharacter.ProficiencyArmorArray.Empty();
	NewCharacter.ProficiencyToolArray.Empty();
	NewCharacter.AcquiredSpells.Empty();
	NewCharacter.SpellBook.Empty();

	NewCharacter.AcquiredSpells.Empty();
	NewCharacter.SpellBook.Empty();
}

void ANewCharacterGameMode::OnFighterChoice(const FString Skill01, const FString Skill02, const FString FightingStyle)
{
	NewCharacter.HitDie = 10;
	NewCharacter.PerLevelHitDie = 10;
	NewCharacter.ClassLevelMap.Emplace(EClass::Fighter, 1);
	NewCharacter.Class = EClass::Fighter;
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
	AddSkillProficiency(GetSkillFromString(Skill01));
	AddSkillProficiency(GetSkillFromString(Skill02));
	
	// Choose fighting style: Archery, Defense, Dueling
	if (GetFightingStyleFromString(FightingStyle) != EFeat::UndefinedFeat)
	{
		NewCharacter.FeatArray.Add(GetFightingStyleFromString(FightingStyle));
	}

	// Add Second Wind to spells
	NewCharacter.AcquiredSpells.Add("g_secondwind", EAbilityType::Constitution);
	NewCharacter.AcquiredSpells.Add("g_mainattack", EAbilityType::UndefiniedAbility);

	// Add some equipment
	NewCharacter.Inventory.Add("w_longsword", 1);
	NewCharacter.Inventory.Add("a_plate", 1);
}

void ANewCharacterGameMode::RevertFighterChoice(const FString Skill01, const FString Skill02, const FString FightingStyle)
{
	// They get overriden anyway
	NewCharacter.HitDie = 0; 
	NewCharacter.PerLevelHitDie = 0;
	NewCharacter.ClassLevelMap.Emplace(EClass::Fighter, 0);
	NewCharacter.Class = EClass::UndefinedClass;
	// Saving Throws:
	RemoveAbilitySavingThrowProficiency(EAbilityType::Strength);
	RemoveAbilitySavingThrowProficiency(EAbilityType::Constitution);

	// No Race gives Simple/Martial or Shield bonuses so these can be safely removed
	RemoveWeaponProficiency(EWeapon::MartialWeapons);
	RemoveWeaponProficiency(EWeapon::SimpleWeapons);
	RemoveWeaponProficiency(EWeapon::Shield);

	// Remove Armors these can also be safely removed
	RemoveArmorProficiency(EArmor::HeavyArmor);
	RemoveArmorProficiency(EArmor::MediumArmor);
	RemoveArmorProficiency(EArmor::LightArmor);


	// Reset skills, if elf add perception because of "Keen Senses"
	RevertSkillMap();
	if (NewCharacter.Race == ERace::Elf)
	{
		AddSkillProficiency(ESkill::Perception);
	}

	// Remove Fighting Style
	NewCharacter.FeatArray.RemoveSingle(EFeat::FightingStyle_Archery);
	NewCharacter.FeatArray.RemoveSingle(EFeat::FightingStyle_Defense);
	NewCharacter.FeatArray.RemoveSingle(EFeat::FightingStyle_Dueling);
	// Remove Second Wind spell
	NewCharacter.AcquiredSpells.Remove("g_secondwind");
	NewCharacter.AcquiredSpells.Remove("g_mainattack");
	// Remove Items
	NewCharacter.Inventory.Remove("w_longsword");
	NewCharacter.Inventory.Remove("a_plate");
}

void ANewCharacterGameMode::OnRogueChoice(const FString Skill01, const FString Skill02)
{
	NewCharacter.HitDie = 8;
	NewCharacter.PerLevelHitDie = 5;
	NewCharacter.ClassLevelMap.Emplace(EClass::Rogue, 1);
	NewCharacter.Class = EClass::Rogue;
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

	AddSkillProficiency(GetSkillFromString(Skill01));
	AddSkillProficiency(GetSkillFromString(Skill02));

	//Saving Throws: Dexterity, Intelligence
	AddAbilitySavingThrowProficiency(EAbilityType::Dexterity);
	AddAbilitySavingThrowProficiency(EAbilityType::Intelligence);
	// Skills: Choose 4 from Acrobatics, Athletics, Deception, Insight, Intimidation, Investigation, Perception, Performance, Persuasion, Sleight of Hand, and Stealth
	NewCharacter.AcquiredSpells.Add("g_mainattack", EAbilityType::UndefiniedAbility);

	NewCharacter.Inventory.Add("w_dagger", 1);
	NewCharacter.Inventory.Add("a_leather", 1);
}

void ANewCharacterGameMode::RevertRogueChoice()
{
	NewCharacter.HitDie = 0;
	NewCharacter.PerLevelHitDie = 0;
	NewCharacter.ClassLevelMap.Emplace(EClass::Fighter, 0);
	NewCharacter.Class = EClass::UndefinedClass;

	RemoveWeaponProficiency(EWeapon::Shortswords);
	RemoveWeaponProficiency(EWeapon::HandCrossbows);
	RemoveWeaponProficiency(EWeapon::Longswords);
	RemoveWeaponProficiency(EWeapon::Rapiers);
	RemoveWeaponProficiency(EWeapon::SimpleWeapons);

	RevertSkillMap();
	if (NewCharacter.Race == ERace::Elf)
	{
		AddSkillProficiency(ESkill::Perception);
	}

	NewCharacter.ProficiencyToolArray.Remove(ETool::ThievesTools);
	NewCharacter.AcquiredSpells.Remove("g_mainattack");
}

void ANewCharacterGameMode::OnWizardChoice(const FString Skill01, const FString Skill02)
{
	NewCharacter.HitDie = 6;
	NewCharacter.PerLevelHitDie = 6;
	NewCharacter.ClassLevelMap.Emplace(EClass::Wizard, 1);
	NewCharacter.Class = EClass::Wizard;
	NewCharacter.SpellcastingAbility = EAbilityType::Intelligence;
	//Armor: None
	
	//Weapons: Daggers, Darts, Quarterstaffs, Light Crossbow
	AddWeaponProficiency(EWeapon::Daggers);
	AddWeaponProficiency(EWeapon::Darts);
	AddWeaponProficiency(EWeapon::Quarterstaffs);
	AddWeaponProficiency(EWeapon::LightCrossbows);
	//Tools: None
	//Saving Throws: Strength, Constitution
	AddAbilitySavingThrowProficiency(EAbilityType::Wisdom);
	AddAbilitySavingThrowProficiency(EAbilityType::Intelligence);
	
	//Skills: Choose 2
	AddSkillProficiency(GetSkillFromString(Skill01));
	AddSkillProficiency(GetSkillFromString(Skill02));

	// Add some spells
	NewCharacter.SpellBook.Add("fireball", EAbilityType::Intelligence);
	NewCharacter.SpellBook.Add("lightningstrike", EAbilityType::Intelligence);
	NewCharacter.SpellBook.Add("firebreath", EAbilityType::Intelligence);
	NewCharacter.SpellBook.Add("curewounds", EAbilityType::Intelligence);

	NewCharacter.AcquiredSpells.Add("g_restore", EAbilityType::UndefiniedAbility);
	NewCharacter.AcquiredSpells.Add("g_mainattack", EAbilityType::UndefiniedAbility);
	// Add some equipment
}

void ANewCharacterGameMode::RevertWizardChoice()
{
	NewCharacter.HitDie = 0;
	NewCharacter.PerLevelHitDie = 0;
	NewCharacter.ClassLevelMap.Emplace(EClass::Fighter, 0);
	NewCharacter.Class = EClass::UndefinedClass;

	RemoveWeaponProficiency(EWeapon::Daggers);
	RemoveWeaponProficiency(EWeapon::Darts);
	RemoveWeaponProficiency(EWeapon::Quarterstaffs);
	RemoveWeaponProficiency(EWeapon::LightCrossbows);

	RemoveAbilitySavingThrowProficiency(EAbilityType::Wisdom);
	RemoveAbilitySavingThrowProficiency(EAbilityType::Intelligence);

	NewCharacter.SpellBook.Remove("fireball");
	NewCharacter.SpellBook.Remove("lightningstrike");
	NewCharacter.SpellBook.Remove("firebreath");
	NewCharacter.SpellBook.Remove("curewounds");
	
	NewCharacter.AcquiredSpells.Remove("g_restore");
	NewCharacter.AcquiredSpells.Remove("g_mainattack");
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
	if (SkillProficiencyToAdd != ESkill::UndefinedSkill)
	{
		NewCharacter.SkillMap.Emplace(SkillProficiencyToAdd, true);
	}
}

void ANewCharacterGameMode::AddResistanceProficiency(const TEnumAsByte<EDamageType> ResistanceProficiencyToAdd)
{
	NewCharacter.DamageResistanceArray.AddUnique(ResistanceProficiencyToAdd);
}

void ANewCharacterGameMode::AddAbilitySavingThrowProficiency(const TEnumAsByte<EAbilityType> AbilityTypeToAdd)
{
	NewCharacter.SavingThrowProficiencyArray.AddUnique(AbilityTypeToAdd);
}

bool ANewCharacterGameMode::IsProficientWith(const TEnumAsByte<ESkill> skill)
{
	if (NewCharacter.SkillMap.Contains(skill))
	{
		if (*NewCharacter.SkillMap.Find(skill) == true)
		{
			return true;
		}
	}
	return false;
}

void ANewCharacterGameMode::FinalizeAbilities(const int32 Str, const int32 Dex, const int32 Con, const int32 Int, const int32 Wis, const int32 Cha)
{
	NewCharacter.Strength = Str;
	NewCharacter.Dexterity = Dex;
	NewCharacter.Constitution = Con;
	NewCharacter.Intelligence = Int;
	NewCharacter.Wisdom = Wis;
	NewCharacter.Charisma = Cha;
}

void ANewCharacterGameMode::SaveCharacter()
{
	NewCharacter.CharacterName = CharacterName;
	if (SaveObject != nullptr)
	{
		UPersistenceSaveObject* Persist = Cast<UPersistenceSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PersistentDataSlot"), 0));
		if (!Persist)
		{
			Persist = Cast<UPersistenceSaveObject>(UGameplayStatics::CreateSaveGameObject(UPersistenceSaveObject::StaticClass())); // If there is no Persistence yet then we create one
			UE_LOG(LogTemp, Warning, TEXT("Created new persistence"));
		}

		//Save the persistent values of a map
		SaveObject->SavedCharacterStruct = NewCharacter;

		FString SlotName = "CharacterSave" + FString::FromInt(Persist->CharacterNextIndex + 1);

		if (UGameplayStatics::SaveGameToSlot(SaveObject, SlotName, 0))
		{
			Persist->CharacterSlotNames.Add(SlotName);
			UE_LOG(LogTemp, Warning, TEXT("Saved %s to %s"), *CharacterName, *SlotName);
		}
		Persist->CharacterNextIndex++;
		if (UGameplayStatics::SaveGameToSlot(Persist, TEXT("PersistentDataSlot"), 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Saved Persistence"));
		}
	}
}

void ANewCharacterGameMode::RemoveAbilitySavingThrowProficiency(TEnumAsByte<EAbilityType> ProficiencyToRemvoce)
{
	NewCharacter.SavingThrowProficiencyArray.RemoveSingle(ProficiencyToRemvoce);
}

void ANewCharacterGameMode::RemoveWeaponProficiency(TEnumAsByte<EWeapon> WeaponProficiencyToRemove)
{
	NewCharacter.ProficiencyWeaponArray.RemoveSingle(WeaponProficiencyToRemove);
}

void ANewCharacterGameMode::RemoveArmorProficiency(TEnumAsByte<EArmor> ArmorProficiencyToRemove)
{
	NewCharacter.ProficiencyArmorArray.RemoveSingle(ArmorProficiencyToRemove);
}

void ANewCharacterGameMode::RemoveSkillProficiency(const TEnumAsByte<ESkill> SkillToRemove)
{
	if (SkillToRemove != ESkill::UndefinedSkill)
	{
		NewCharacter.SkillMap.Emplace(SkillToRemove, false);
	}
}

TEnumAsByte<ESkill> ANewCharacterGameMode::GetSkillFromString(const FString skillAsString)
{
	if (skillAsString == "Athletics")
	{
		return ESkill::Athletics;
	}
	else if (skillAsString == "Acrobatics")
	{
		return ESkill::Acrobatics;
	}
	else if (skillAsString == "Animal Handling")
	{
		return ESkill::AnimalHandling;
	}
	else if (skillAsString == "Arcana")
	{
		return ESkill::Arcana;
	}
	else if (skillAsString == "Deception")
	{
		return ESkill::Deception;
	}
	else if (skillAsString == "History")
	{
		return ESkill::History;
	}
	else if (skillAsString == "Insight")
	{
		return ESkill::Insight;
	}
	else if (skillAsString == "Intimidation")
	{
		return ESkill::Intimidation;
	}
	else if (skillAsString == "Investigation")
	{
		return ESkill::Investigation;
	}
	else if (skillAsString == "Medicine")
	{
		return ESkill::Medicine;
	}
	else if (skillAsString == "Nature")
	{
		return ESkill::Nature;
	}
	else if (skillAsString == "Perception")
	{
		return ESkill::Perception;
	}
	else if (skillAsString == "Performance")
	{
		return ESkill::Performance;
	}
	else if (skillAsString == "Persuasion")
	{
		return ESkill::Persuasion;
	}
	else if (skillAsString == "Religion")
	{
		return ESkill::Religion;
	}
	else if (skillAsString == "Sleight Of Hand")
	{
		return ESkill::SleightOfHand;
	}
	else if (skillAsString == "Stealth")
	{
		return ESkill::Stealth;
	}
	else if (skillAsString == "Survival")
	{
		return ESkill::Survival;
	}
	return ESkill::UndefinedSkill;
}

TEnumAsByte<EFeat> ANewCharacterGameMode::GetFightingStyleFromString(const FString fightingStyleAsString)
{
	if (fightingStyleAsString=="Archery")
	{
		return EFeat::FightingStyle_Archery;
	}
	else if (fightingStyleAsString == "Defense")
	{
		return EFeat::FightingStyle_Defense;
	}
	else if (fightingStyleAsString == "Dueling")
	{
		return EFeat::FightingStyle_Dueling;
	}
	return EFeat::UndefinedFeat;
}

void ANewCharacterGameMode::RevertSkillMap()
{
	NewCharacter.SkillMap.Emplace(ESkill::Acrobatics, false);
	NewCharacter.SkillMap.Emplace(ESkill::Athletics, false);
	NewCharacter.SkillMap.Emplace(ESkill::AnimalHandling, false);
	NewCharacter.SkillMap.Emplace(ESkill::Arcana, false);
	NewCharacter.SkillMap.Emplace(ESkill::Deception, false);
	NewCharacter.SkillMap.Emplace(ESkill::History, false);
	NewCharacter.SkillMap.Emplace(ESkill::Insight, false);
	NewCharacter.SkillMap.Emplace(ESkill::Intimidation, false);
	NewCharacter.SkillMap.Emplace(ESkill::Investigation, false);
	NewCharacter.SkillMap.Emplace(ESkill::Medicine, false);
	NewCharacter.SkillMap.Emplace(ESkill::Nature, false);
	NewCharacter.SkillMap.Emplace(ESkill::Perception, false);
	NewCharacter.SkillMap.Emplace(ESkill::Performance, false);
	NewCharacter.SkillMap.Emplace(ESkill::Persuasion, false);
	NewCharacter.SkillMap.Emplace(ESkill::Religion, false);
	NewCharacter.SkillMap.Emplace(ESkill::SleightOfHand, false);
	NewCharacter.SkillMap.Emplace(ESkill::Stealth, false);
	NewCharacter.SkillMap.Emplace(ESkill::Survival, false);
}
