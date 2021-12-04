// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/BaseWeaponItem.h"

UBaseWeaponItem::UBaseWeaponItem()
{
	ItemType = EItemType::ItemType_Weapon;
	bIsStackable = false;
	ItemStack = 1;
	MagicBonus = 0;
}
