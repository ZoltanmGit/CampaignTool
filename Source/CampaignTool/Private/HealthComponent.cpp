// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

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
void UHealthComponent::HandleTakeDamage(float Damage)
{
	SetCurrentHealth(GetCurrentHealth() - Damage);
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

