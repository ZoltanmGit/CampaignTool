// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAoeTargetAbility.h"

UBaseAoeTargetAbility::UBaseAoeTargetAbility()
{
	TargetType = ETargetType::AOE;
}

void UBaseAoeTargetAbility::Execute()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Area of Effect Execute called"));
}
