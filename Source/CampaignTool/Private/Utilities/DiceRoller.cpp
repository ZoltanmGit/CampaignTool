// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/DiceRoller.h"

int32 UDiceRoller::Roll(int32 dice, int32 multiplier)
{
	int32 result = 0;
	FRandomStream stream(FMath::Rand());
	for (int i = 0; i < multiplier; i++)
	{
		result += stream.RandRange(1, dice);
	}
	return result;
}
