// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponItem.h"

UBaseWeaponItem::UBaseWeaponItem()
{
	ItemType = EItemType::ItemType_Weapon;
	bIsStackable = false;
	ItemStack = 1;
	MagicBonus = 0;
}
