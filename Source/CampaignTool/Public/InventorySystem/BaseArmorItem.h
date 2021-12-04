// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BaseArmorItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CAMPAIGNTOOL_API UBaseArmorItem : public UBaseItem
{
	GENERATED_BODY()
	
public:
	UBaseArmorItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ArmorProperties)
		TEnumAsByte<EArmor> ArmorType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ArmorProperties)
		int32 ArmorClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ArmorProperties)
		int32 MaxDexterity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ArmorProperties)
		bool bStealthDisadvantage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ArmorProperties)
		bool bHasStrengthRequirement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ArmorProperties, meta = (EditCondition = "bHasStrengthRequirement"))
		int32 StrengthRequirement;
};
