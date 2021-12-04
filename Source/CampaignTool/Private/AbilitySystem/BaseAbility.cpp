// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbility.h"
#include "Character/BaseCharacter.h"

UBaseAbility::UBaseAbility()
{
	OptionalRange = 0;
}

void UBaseAbility::OnExecute_Implementation() { } // This is not redundant.