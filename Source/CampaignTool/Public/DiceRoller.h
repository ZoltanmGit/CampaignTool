// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DiceRoller.generated.h"

UCLASS()
class CAMPAIGNTOOL_API UDiceRoller : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		int32 Roll(int32 dice,int32 multiplier = 1);
};
