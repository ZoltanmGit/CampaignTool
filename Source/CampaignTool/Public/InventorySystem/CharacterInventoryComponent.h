// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utilities/AttributeEnums.h"
#include "CharacterInventoryComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CAMPAIGNTOOL_API UCharacterInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/** General **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = General)
		class ABaseCharacter* Owner;
	/** Statistics **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Statistics)
		int32 ArmorClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Statistics)
		int32 MainHandAttackBonus;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Statistics)
		int32 OffHandAttackBonus;
	/** Equipment **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseWeaponItem* MainHandWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseWeaponItem* OffHandWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseArmorItem* Armor;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseAccessoryItem* Helmet;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseAccessoryItem* Ring1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseAccessoryItem* Ring2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseAccessoryItem* Cloak;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseAccessoryItem* Boots;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseAccessoryItem* Gloves;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		TArray<class UBaseAmmunitionItem> EquippedAmmunitionArray; // Array of 2
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		TArray<class UBaseConsumableItem> EquippedConsumableArray;*/ // Array of 3
public:
	/** Functions **/
	UFUNCTION(BlueprintCallable)
		void UpdateArmorClass();
	UFUNCTION(BlueprintCallable)
		void UpdateMainAttackBonus();
	UFUNCTION(BlueprintCallable)
		void UpdateOffhandAttackBonus();

};
