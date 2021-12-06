// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEquippableItem.h"
#include "BaseWeaponItem.generated.h"

/**
 * 
 */



UCLASS(Blueprintable)
class CAMPAIGNTOOL_API UBaseWeaponItem : public UBaseEquippableItem
{
	GENERATED_BODY()
public:
	UBaseWeaponItem();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		TEnumAsByte<EWeapon> WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		int32 DamageDie;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		int32 DamageDieMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		bool bIsMagical;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		int32 MagicBonus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		TEnumAsByte<EDamageType> DamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		int32 WeaponRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		TEnumAsByte<ERangeType> RangeType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		bool bIsFinesseWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		bool bUsesAmmunition;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties, meta = (EditCondition = "bUsesAmmunition"))
		TEnumAsByte<EAmmunitionType> AmmunitionType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties)
		bool bHasSpecificEnemy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponProperties, meta = (EditCondition = "bHasSpecificEnemy"))
		TEnumAsByte<ECreatureType> EnemyType;
};
