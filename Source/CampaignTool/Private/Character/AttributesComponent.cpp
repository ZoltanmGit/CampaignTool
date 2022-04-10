// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AttributesComponent.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{
	Stats.CharacterName = "DefaultConstructorCharacterName";
}


// Called when the game starts
void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributesComponent::InitComponent(FCharacterStruct charstruct)
{
	Stats = charstruct;
}

int32 UAttributesComponent::GetModifier(const TEnumAsByte<EAbilityType> Ability)
{
	switch (Ability)
	{
	case EAbilityType::Strength:
		return FMath::FloorToInt((Stats.Strength - 10) / 2);
		break;
	case EAbilityType::Dexterity:
		return FMath::FloorToInt((Stats.Dexterity - 10) / 2);
		break;
	case EAbilityType::Constitution:
		return FMath::FloorToInt((Stats.Constitution - 10) / 2);
		break;
	case EAbilityType::Intelligence:
		return FMath::FloorToInt((Stats.Intelligence - 10) / 2);
		break;
	case EAbilityType::Wisdom:
		return FMath::FloorToInt((Stats.Wisdom - 10) / 2);
		break;
	case EAbilityType::Charsima:
		return FMath::FloorToInt((Stats.Charisma - 10) / 2);
		break;
	default:
		return 0;
		break;
	}
	return 0;
}
int32 UAttributesComponent::GetSavingThrowModifier(const TEnumAsByte<EAbilityType> Ability)
{
	int32 result = 0;
	result += GetModifier(Ability);

	if (Stats.SavingThrowProficiencyArray.Contains(Ability))
	{
		result += Stats.ProficiencyBonus;
	}
	return result;
}


int32 UAttributesComponent::GetProficiencyBonus()
{
	return FMath::Clamp(Stats.ProficiencyBonus, 0, 6); // I think technically the minimum is +2 but it needs some digging
}

int32 UAttributesComponent::GetClassLevel(const TEnumAsByte<EClass> classToFind)
{
	if (Stats.ClassLevelMap.Contains(classToFind))
	{
		return *Stats.ClassLevelMap.Find(classToFind);
	}
	return 0;
}

bool UAttributesComponent::IsProficientWith(TEnumAsByte<EWeapon> weapon)
{
	if (Stats.ProficiencyWeaponArray.Contains(weapon))
	{
		return true;
	}
	return false;
}
bool UAttributesComponent::IsProficientWith(TEnumAsByte<EArmor> armor)
{
	if (Stats.ProficiencyArmorArray.Contains(armor))
	{
		return true;
	}
	return false;
}
bool UAttributesComponent::IsProficientWith(TEnumAsByte<ETool> tool)
{
	if (Stats.ProficiencyToolArray.Contains(tool))
	{
		return true;
	}
	return false;
}

bool UAttributesComponent::IsImmuneTo(TEnumAsByte<EDamageType> damageType)
{
	if (Stats.DamageImmunities.Contains(damageType))
	{
		return true;
	}
	return false;
}
bool UAttributesComponent::IsImmuneTo(TEnumAsByte<ECondition> condition)
{
	if (Stats.ConditionImmunities.Contains(condition))
	{
		return true;
	}
	return false;
}

bool UAttributesComponent::IsResistantTo(TEnumAsByte<EDamageType> damageType)
{
	if (Stats.DamageResistanceArray.Contains(damageType))
	{
		return true;
	}
	return false;
}
