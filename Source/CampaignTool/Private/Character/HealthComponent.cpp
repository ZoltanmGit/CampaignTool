// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HealthComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/AttributesComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	fullHealth = 100.0f;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}
void UHealthComponent::HandleDamage(ABaseCharacter* Instigator ,float Damage, EDamageType DamageType)
{
	switch (DamageType)
	{
	case Healing:
		//If it's a healing spell then heal
		if (Owner != nullptr)
		{
			if (Owner->CharacterAttributes->Stats.CreatureType != ECreatureType::TUndead && Owner->CharacterAttributes->Stats.CreatureType != ECreatureType::TConstruct)
			{
				// If it's not undead nor construct than the character can be healed
				SetCurrentHealth(GetCurrentHealth() + Damage);
			}
		}
		Owner->OnHealthChange();
		break;
	default:
		if (Owner != nullptr)
		{
			if (Owner->CharacterAttributes->Stats.DamageImmunities.Contains(DamageType))
			{
				// If immunte to DamageType then skip
			}
			else if(Owner->CharacterAttributes->Stats.DamageResistanceArray.Contains(DamageType))
			{
				//If resistant then take half damage
				SetCurrentHealth(GetCurrentHealth() - (FMath::Floor(Damage / 2)));
			}
			else
			{
				//If not resistant nor immune than take the full damage | TODO: Saving throw
				SetCurrentHealth(GetCurrentHealth() - Damage);
			}
			Owner->OnHealthChange();
		}
		break;
	}
}

float UHealthComponent::GetCurrentHealth()
{
	return currentHealth;
}

float UHealthComponent::GetFullHealth()
{
	return fullHealth;
}

float UHealthComponent::GetArmorClass()
{
	return 0.0f;
}

float UHealthComponent::GetHealthPercent()
{
	return currentHealth/fullHealth;
}

void UHealthComponent::SetCurrentHealth(float value)
{
	currentHealth = FMath::Clamp<float>(value, 0, fullHealth);
}

void UHealthComponent::SetFullHealth(float value)
{
	fullHealth = value;
}

