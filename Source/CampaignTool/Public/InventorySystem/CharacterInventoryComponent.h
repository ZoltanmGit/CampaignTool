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
	int32 mainHandDamageBonus;
	int32 offHandAttackBonus;
	int32 offHandDamagebonus;

public:
	/** General **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = General)
		class ABaseCharacter* Owner;
	/** Equipment **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		TMap<TEnumAsByte<EEquipmentSlot>, class UBaseEquippableItem*> EquipmentMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		bool bHasShieldEquipped;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		bool bHasMainHand;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
		bool bHasOffhand;
	
	/** MainInventoryArray **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Invnetory)
		TArray<class UBaseItem*> Inventory;
public:
	/** Get **/
	UFUNCTION(BlueprintCallable)
		class UBaseEquippableItem* GetEquipment(TEnumAsByte<EEquipmentSlot> EquipmentSlot);
	UFUNCTION(BlueprintCallable)
		int32 GetArmorClass();
	UFUNCTION(BlueprintCallable)
		int32 GetMainAttackBonus();
	UFUNCTION(BlueprintCallable)
		int32 GetOffhandAttackBonus();
	UFUNCTION(BlueprintCallable)
		int32 GetMainDamageBonus();
	UFUNCTION(BlueprintCallable)
		int32 GetOffhandDamageBonus();
	UFUNCTION(BlueprintCallable)
		class UBaseWeaponItem* GetMainHandWeapon();
	UFUNCTION(BlueprintCallable)
		class UBaseEquippableItem* GetOffhandWeapon();
	
	/** Inventory Management **/
	UFUNCTION(BlueprintCallable)
		bool ResolveEquipItem(class UBaseEquippableItem* itemToEquip);
	UFUNCTION(BlueprintCallable)
		bool ResolveArmorEquipItem(class UBaseEquippableItem* armorToEquip);
	UFUNCTION(BlueprintCallable)
		bool ResolveShieldEquipItem(class UBaseEquippableItem* shieldToEquip);
	UFUNCTION(BlueprintCallable)
		bool ResolveWeaponEquipItem(class UBaseEquippableItem* weaponToEquip);
	UFUNCTION(BlueprintCallable)
		bool EquipItem(class UBaseEquippableItem* itemToEquip, TEnumAsByte<EEquipmentSlot> toSlot);
	UFUNCTION(BlueprintCallable)
		bool EquipItemToSlot(class UBaseEquippableItem* itemToEquip, TEnumAsByte<EEquipmentSlot> toSlot);
	UFUNCTION(BlueprintCallable)
		bool UnequipItemFromSlot(class UBaseEquippableItem* itemToUnequip, TEnumAsByte<EEquipmentSlot> fromSlot);
	UFUNCTION(BlueprintCallable)
		bool RemoveItemFromInventory(class UBaseItem* itemToRemove);
	UFUNCTION(BlueprintCallable)
		bool RemoveItemFromInventoryByIndex(int32 index);
	UFUNCTION(BlueprintCallable)
		bool AddItemToInventory(class UBaseItem* itemToAdd);
	UFUNCTION(BlueprintCallable)
		bool SwapEquippedWithInventory(class UBaseEquippableItem* equippedItem, class UBaseEquippableItem* itemToEquip );
	UFUNCTION(BlueprintCallable)
		bool AddItemFromItemStorage(FString itemCode);
	
	/** Update Functions **/
	UFUNCTION()
		void UpdateArmorClass();
	UFUNCTION()
		void UpdateMainAttackBonus();
	UFUNCTION()
		void UpdateMainDamageBonus();
	UFUNCTION()
		void UpdateOffhandDamageBonus();
	UFUNCTION()
		void UpdateOffhandAttackBonus();
private:
	int32 GetFirstNullIndex();
	int32 GetFirstNonFullStackIndex(FString ItemName);
	UPROPERTY()
		class UBaseEquippableItem* BucketEquippable;
};
