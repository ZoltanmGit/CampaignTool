// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSingleTargetAbility.h"

UBaseSingleTargetAbility::UBaseSingleTargetAbility()
{
	TargetType = ETargetType::Single;
}

void UBaseSingleTargetAbility::Execute()
{
	UE_LOG(LogTemp, Warning, TEXT("Single target spell executed"));
}
