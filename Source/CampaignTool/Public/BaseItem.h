// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributeEnums.h"
#include "BaseItem.generated.h"

UENUM(BlueprintType)
enum EItemType
{
	ItemType_Armor UMETA(DisplayName = "Armor"),
	ItemType_Weapon UMETA(DisplayName = "Weapon"),
	ItemType_Consumable UMETA(DisplayName = "Consumable"),
	ItemType_Accessory UMETA(DisplayName = "Accessory"),
	ItemType_Ammunation UMETA(DisplayName = "Ammunation"),
	ItemType_Shield UMETA(DisplayName = "Shield")
};

UCLASS()
class CAMPAIGNTOOL_API UBaseItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemProperties)
		FString ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemProperties)
		FString ItemDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemProperties)
		bool bIsStackable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemProperties)
		int32 ItemStack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemProperties)
		TEnumAsByte<EItemType> ItemType;

	UFUNCTION(BlueprintCallable)
		void AddToStack(int32 amount);
	UFUNCTION(BlueprintCallable)
		void RemoveFromStack(int32 amount);
	UFUNCTION(BlueprintCallable)
		void SetStack(int32 amount);
};
