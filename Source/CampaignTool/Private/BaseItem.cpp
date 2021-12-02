// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

void UBaseItem::AddToStack(int32 amount)
{
	if (bIsStackable)
	{
		ItemStack = FMath::Clamp(ItemStack + amount, 1, 16);
	}
}

void UBaseItem::RemoveFromStack(int32 amount)
{
	if (bIsStackable)
	{
		ItemStack = FMath::Clamp(ItemStack - amount, 1, 16);
	}
}

void UBaseItem::SetStack(int32 amount)
{
}
