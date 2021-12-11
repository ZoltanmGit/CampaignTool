// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/CharacterInventoryComponent.h"
#include "InventorySystem/BaseArmorItem.h"
#include "InventorySystem/BaseWeaponItem.h"
#include "Character/BaseCharacter.h"
#include "Character/AttributesComponent.h"

// Sets default values for this component's properties
UCharacterInventoryComponent::UCharacterInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCharacterInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UCharacterInventoryComponent::UpdateArmorClass()
{
	ArmorClass = 0;
	/** First we check if the character is wearing armor **/
	if (Armor != nullptr)
	{
		/** If he's wearing armor then we start off by it's armor value **/
		ArmorClass = Armor->ArmorClass;
		/** We add Dexterity if applicable **/
		if (Armor->ArmorType != EArmor::HeavyArmor)
		{
			ArmorClass += FMath::Clamp(Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity), -5, Armor->MaxDexterity);
		}
	}
	else
	{
		/** If they're not wearing any armor then all hell breaks loose oof **/
	}
}

void UCharacterInventoryComponent::UpdateMainAttackBonus()
{
	MainHandAttackBonus = 0;
	/** If we have a weapon equipped **/
	if (MainHandWeapon != nullptr)
	{
		/** If the weapon has finesse then we use STR or DEX whichever is highest **/
		if (!MainHandWeapon->bIsFinesseWeapon)
		{
			if (Owner->CharacterAttributes->GetModifier(EAbilityType::Strength) > Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity))
			{
				MainHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Strength);
			}
			else //If it's equal than it doesn't matter
			{
				MainHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity);
			}
		}
		/** If it doesn't have finesse then we just add the STR modifier **/
		else 
		{
			MainHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity);
		}
		/** If he's proficient then we also add the proficiency bonus **/
		if (Owner->CharacterAttributes->IsProficientWith(MainHandWeapon->WeaponType))
		{
			MainHandAttackBonus += Owner->CharacterAttributes->GetProficiencyBonus();
		}
	}
	else // it'd count as unarmed
	{

	}
}
void UCharacterInventoryComponent::UpdateOffhandAttackBonus()
{
	if (OffHandWeapon != nullptr)
	{

	}
}