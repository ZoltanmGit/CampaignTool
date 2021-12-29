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
private:
	
	/** Statistics **/
	int32 armorClass;
	int32 mainHandAttackBonus;
	int32 offHandAttackBonus;

public:
	/** General **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = General)
		class ABaseCharacter* Owner;
	/** Equipment **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseWeaponItem* MainHandWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseEquippableItem* OffHandWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		class UBaseArmorItem* Armor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Invnetory)
		TArray<class UBaseItem*> ItemArray;
public:
	/** Functions **/
	UFUNCTION(BlueprintCallable)
		int32 GetArmorClass();
	UFUNCTION(BlueprintCallable)
		int32 GetMainAttackBonus();
	UFUNCTION(BlueprintCallable)
		int32 GetOffhandAttackBonus();
	UFUNCTION(BlueprintCallable)
		void EquipItem(class UBaseItem* itemToEquip);
	UFUNCTION(BlueprintCallable)
		void UnequipItem(class UBaseItem* itemToUnequip);

	UFUNCTION(BlueprintCallable)
		class UBaseWeaponItem* GetMainHandWeapon();
	UFUNCTION(BlueprintCallable)
		class UBaseEquippableItem* GetOffhandWeapon();
	/** Update Functions **/
	UFUNCTION()
		void UpdateArmorClass();
	UFUNCTION()
		void UpdateMainAttackBonus();
	UFUNCTION()
		void UpdateOffhandAttackBonus();
};
