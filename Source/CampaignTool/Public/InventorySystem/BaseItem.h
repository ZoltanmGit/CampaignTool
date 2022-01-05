// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Utilities/AttributeEnums.h"
#include "BaseItem.generated.h"

UCLASS(Blueprintable)
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemProperties)
		UTexture2D* ItemTexture;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemProperties)
		class ABaseCharacter* ItemOwner;

	UFUNCTION(BlueprintCallable)
		void AddToStack(int32 amount);
	UFUNCTION(BlueprintCallable)
		void RemoveFromStack(int32 amount);
	UFUNCTION(BlueprintCallable)
		void SetStack(int32 amount);
};
