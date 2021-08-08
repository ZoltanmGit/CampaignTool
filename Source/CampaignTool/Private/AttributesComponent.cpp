// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesComponent.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{
}


// Called when the game starts
void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

int32 UAttributesComponent::GetModifier(int32 value)
{
	return (value-10)/2;
}

int32 UAttributesComponent::GetStrength()
{
	return Strength;
}

int32 UAttributesComponent::GetDexterity()
{
	return Dexterity;
}

int32 UAttributesComponent::GetIntelligence()
{
	return Intelligence;
}

int32 UAttributesComponent::GetWisdom()
{
	return Wisdom;
}

int32 UAttributesComponent::GetConstitution()
{
	return Constitution;
}

int32 UAttributesComponent::GetCharisma()
{
	return Charisma;
}

int32 UAttributesComponent::GetAthletics()
{
	return Athletics;
}

int32 UAttributesComponent::GetAcrobatics()
{
	return Acrobatics;
}

int32 UAttributesComponent::GetSleightOfHand()
{
	return SleightOfHand;
}

int32 UAttributesComponent::GetStealth()
{
	return Stealth;
}

int32 UAttributesComponent::GetArcana()
{
	return Arcana;
}

int32 UAttributesComponent::GetHistory()
{
	return History;
}

int32 UAttributesComponent::GetInvestigation()
{
	return Investigation;
}

int32 UAttributesComponent::GetNature()
{
	return Nature;
}

int32 UAttributesComponent::GetReligion()
{
	return Religion;
}

int32 UAttributesComponent::GetAnimalHandling()
{
	return AnimalHandling;
}

int32 UAttributesComponent::GetInsight()
{
	return Insight;
}

int32 UAttributesComponent::GetMedicine()
{
	return Medicine;
}

int32 UAttributesComponent::GetPerception()
{
	return Perception;
}

int32 UAttributesComponent::GetSurvival()
{
	return Survival;
}

int32 UAttributesComponent::GetDeception()
{
	return Deception;
}

int32 UAttributesComponent::GetIntimidation()
{
	return Intimidation;
}

int32 UAttributesComponent::GetPerformance()
{
	return Performance;
}

int32 UAttributesComponent::GetPersuasion()
{
	return Persuasion;
}
