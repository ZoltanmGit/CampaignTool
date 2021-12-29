// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/BaseShieldItem.h"

UBaseShieldItem::UBaseShieldItem()
{
	ItemType = EItemType::ItemType_Shield;
	bIsStackable = false;
	ItemStack = 1;
	MagicBonus = 0;
}