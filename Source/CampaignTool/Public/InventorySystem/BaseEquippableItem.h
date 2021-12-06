// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/BaseItem.h"
#include "BaseEquippableItem.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API UBaseEquippableItem : public UBaseItem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
		void OnEquip();
	UFUNCTION(BlueprintNativeEvent)
		void OnUnEquip();
};
