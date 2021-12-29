// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/BaseEquippableItem.h"
#include "BaseShieldItem.generated.h"


UCLASS(Blueprintable)
class CAMPAIGNTOOL_API UBaseShieldItem : public UBaseEquippableItem
{
	GENERATED_BODY()
	
public:
	UBaseShieldItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ShieldProperty)
		int32 ArmorClassBonus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ShieldProperty)
		int32 MagicBonus;
};
