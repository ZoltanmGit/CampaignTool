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

	UpdateArmorClass();
	// ...
}

void UCharacterInventoryComponent::UpdateArmorClass()
{
	armorClass = Owner->CharacterAttributes->Stats.ArmorClass;
	/** First we check if the character is wearing armor **/
	if (Armor != nullptr)
	{
		/** If he's wearing armor then we start off by it's armor value **/
		armorClass = Armor->ArmorClass;
		/** We add Dexterity if applicable **/
		if (Armor->ArmorType != EArmor::HeavyArmor)
		{
			armorClass += FMath::Clamp(Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity), -5, Armor->MaxDexterity);
		}
	}
	else
	{
		/** If they're not wearing any armor then all hell breaks loose oof **/
	}
}

void UCharacterInventoryComponent::UpdateMainAttackBonus()
{
	mainHandAttackBonus = 0;
	/** If we have a weapon equipped **/
	if (MainHandWeapon != nullptr)
	{
		/** If the weapon has finesse then we use STR or DEX whichever is highest **/
		if (!MainHandWeapon->bIsFinesseWeapon)
		{
			if (Owner->CharacterAttributes->GetModifier(EAbilityType::Strength) > Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity))
			{
				mainHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Strength);
			}
			else //If it's equal than it doesn't matter
			{
				mainHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity);
			}
		}
		/** If it doesn't have finesse then we just add the STR modifier **/
		else 
		{
			mainHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity);
		}
		/** If he's proficient then we also add the proficiency bonus **/
		if (Owner->CharacterAttributes->IsProficientWith(MainHandWeapon->WeaponType))
		{
			mainHandAttackBonus += Owner->CharacterAttributes->GetProficiencyBonus();
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
	else // I think it might count as nothing.
	{

	}
}

int32 UCharacterInventoryComponent::GetArmorClass()
{
	return FMath::Clamp(armorClass,0,100);
}

int32 UCharacterInventoryComponent::GetMainAttackBonus()
{
	return FMath::Clamp(mainHandAttackBonus,-99,99);
}

int32 UCharacterInventoryComponent::GetOffhandAttackBonus()
{
	return FMath::Clamp(offHandAttackBonus, -99, 99);
}
