// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BaseWeaponItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CAMPAIGNTOOL_API UBaseWeaponItem : public UBaseItem
{
	GENERATED_BODY()
public:
	UBaseWeaponItem();

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
};
