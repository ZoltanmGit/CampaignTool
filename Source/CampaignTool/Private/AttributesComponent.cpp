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
}

void UAttributesComponent::InitComponent(FCharacterStruct charstruct)
{
	Stats = charstruct;
}

int32 UAttributesComponent::GetModifier(int32 value)
{
	return (value-10)/2;
}
