// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseArmorItem.h"

UBaseArmorItem::UBaseArmorItem()
{
	ItemType = EItemType::ItemType_Armor;
	bIsStackable = false;
	ItemStack = 1;
}