// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/CharacterInventoryComponent.h"
#include "InventorySystem/BaseArmorItem.h"
#include "InventorySystem/BaseWeaponItem.h"
#include "InventorySystem/BaseShieldItem.h"
#include "Character/BaseCharacter.h"
#include "Character/AttributesComponent.h"

// Sets default values for this component's properties
UCharacterInventoryComponent::UCharacterInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	/** Initialize possible equipment slots **/
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_MainHand, nullptr);
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_Armor, nullptr);
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_Offhand, nullptr);
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_Ring1, nullptr);
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_Ring2, nullptr);
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_Boots, nullptr);
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_Gloves, nullptr);
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_Helmet, nullptr);
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_Ammunition1, nullptr);
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_Ammunition2, nullptr);
	EquipmentMap.Add(EEquipmentSlot::EquipmentSlot_Cape, nullptr);

	Inventory.Init(nullptr, 32);
	bHasMainHand = false;
	bHasOffhand = false;
	bHasShieldEquipped = false;
}

// Called when the game starts
void UCharacterInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

void UCharacterInventoryComponent::UpdateArmorClass()
{
	armorClass = Owner->CharacterAttributes->Stats.ArmorClass;
	UBaseArmorItem* Armor = Cast<UBaseArmorItem>(GetEquipment(EEquipmentSlot::EquipmentSlot_Armor));
	/** First we check if the character is wearing armor **/
	if (Armor != nullptr)
	{
		/** If he's wearing armor then we start off by it's armor value **/
		armorClass = Armor->ArmorClass;
		/** We add Dexterity if applicable **/
		if (Armor->ArmorType != EArmor::HeavyArmor)
		{
			armorClass += FMath::Clamp(Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity), -5, Armor->MaxDexterity);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - UpdateAC: Character is not wearing armor"));
	}

	/** Check if character is wearing any shield **/
	if (bHasShieldEquipped)
	{
		UBaseShieldItem* equippedShield = Cast<UBaseShieldItem>(GetEquipment(EEquipmentSlot::EquipmentSlot_Offhand));
		if (equippedShield != nullptr)
		{
			armorClass += equippedShield->ArmorClassBonus + equippedShield->MagicBonus;
		}
	}
}

void UCharacterInventoryComponent::UpdateMainAttackBonus()
{
	mainHandAttackBonus = 0;
	UBaseWeaponItem* MainHandWeapon = Cast<UBaseWeaponItem>(*EquipmentMap.Find(EEquipmentSlot::EquipmentSlot_MainHand));
	/** If we have a weapon equipped **/
	if (MainHandWeapon != nullptr)
	{
		/** If the weapon has finesse then we use STR or DEX whichever is highest **/
		if (MainHandWeapon->PropertiesArray.Contains(EWeaponProperty::WP_Finesse))
		{
			if (Owner->CharacterAttributes->GetModifier(EAbilityType::Strength) > Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity))
			{
				mainHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Strength);
			}
			else //If it's equal than it doesn't matter
			{
				mainHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity);
			}
		}
		/** If it doesn't have finesse then we just add the STR modifier **/
		else 
		{
			mainHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Strength);
		}
		/** If he's proficient then we also add the proficiency bonus **/
		bool bIsProficientWithAny = false;
		int32 loopVar = 0;
		do
		{
			bIsProficientWithAny = Owner->CharacterAttributes->IsProficientWith(MainHandWeapon->WeaponTypeArray[loopVar]);
			loopVar++;
		} while (!bIsProficientWithAny && loopVar < MainHandWeapon->WeaponTypeArray.Num());
		if (bIsProficientWithAny)
		{
			mainHandAttackBonus += Owner->CharacterAttributes->GetProficiencyBonus();
		}
		/** If the weapon is magical, then we add that to the bonus **/
		if (MainHandWeapon->bIsMagical)
		{
			mainHandAttackBonus += MainHandWeapon->MagicBonus;
		}
	}
	else // it'd count as unarmed
	{

	}
}

void UCharacterInventoryComponent::UpdateMainDamageBonus()
{
	mainHandDamageBonus = 0;
	UBaseWeaponItem* MainHandWeapon = Cast<UBaseWeaponItem>(GetEquipment(EEquipmentSlot::EquipmentSlot_MainHand));
	if (MainHandWeapon != nullptr)
	{
		/** Determine Finesse **/
		if (MainHandWeapon->PropertiesArray.Contains(EWeaponProperty::WP_Finesse))
		{
			if (Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity) > Owner->CharacterAttributes->GetModifier(EAbilityType::Strength))
			{
				mainHandDamageBonus += Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity);
			}
			else
			{
				mainHandDamageBonus += Owner->CharacterAttributes->GetModifier(EAbilityType::Strength);
			}
		}
		else
		{
			mainHandDamageBonus += Owner->CharacterAttributes->GetModifier(EAbilityType::Strength);
		}

		/** Determine Magical Bonus **/
		if (MainHandWeapon->bIsMagical)
		{
			mainHandDamageBonus += MainHandWeapon->MagicBonus;
		}
	}
}

void UCharacterInventoryComponent::UpdateOffhandDamageBonus()
{

}

void UCharacterInventoryComponent::UpdateOffhandAttackBonus()
{
	offHandAttackBonus = 0;
	/** In the event that the character is wearing a shield then we skip **/
	if (!bHasShieldEquipped)
	{
		UBaseWeaponItem* OffhandWeapon = Cast<UBaseWeaponItem>(*EquipmentMap.Find(EEquipmentSlot::EquipmentSlot_Offhand));
		if (OffhandWeapon != nullptr)
		{
			/** For two weapon fighting we only consider negative modifiers **/
			if (OffhandWeapon->PropertiesArray.Contains(EWeaponProperty::WP_Finesse))
			{
				if (Owner->CharacterAttributes->GetModifier(EAbilityType::Strength) > Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity))
				{
					offHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Strength);
				}
				else //If it's equal than it doesn't matter
				{
					offHandAttackBonus = Owner->CharacterAttributes->GetModifier(EAbilityType::Dexterity);
				}
			}
			else
			{
				offHandAttackBonus += FMath::Clamp(Owner->CharacterAttributes->GetModifier(EAbilityType::Strength), -5, 0);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - UpdateOffhandAttackBonus(): No offhand equipped."));
		}
	}
}

int32 UCharacterInventoryComponent::GetFirstNullIndex()
{
	/** Returns the index of the first Inventory slot that holds no items and is therefore nullptr **/
	int32 loopVar = 0;
	do
	{
		if (Inventory[loopVar] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - GetFirstNullIndex(): First null index is %i"), loopVar);
			return loopVar;
		}
		loopVar++;
	} while (loopVar < Inventory.Num());
	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - GetFirstNullIndex(): No null index was found in inventory."), loopVar);
	return -1;
}

int32 UCharacterInventoryComponent::GetFirstNonFullStackIndex(FString ItemName)
{
	return int32();
}

UBaseEquippableItem* UCharacterInventoryComponent::GetEquipment(TEnumAsByte<EEquipmentSlot> EquipmentSlot)
{
	return *EquipmentMap.Find(EquipmentSlot);
}

int32 UCharacterInventoryComponent::GetArmorClass()
{
	return FMath::Clamp(armorClass,0,100);
}

int32 UCharacterInventoryComponent::GetMainAttackBonus()
{
	int32 MainAttackBonus = 0;
	return FMath::Clamp(mainHandAttackBonus,-99,99);
}

int32 UCharacterInventoryComponent::GetOffhandAttackBonus()
{
	return FMath::Clamp(offHandAttackBonus, -99, 99);
}

int32 UCharacterInventoryComponent::GetMainDamageBonus()
{
	return mainHandDamageBonus;
}

int32 UCharacterInventoryComponent::GetOffhandDamageBonus()
{
	return offHandDamagebonus;
}

bool UCharacterInventoryComponent::EquipItem(UBaseEquippableItem* itemToEquip, TEnumAsByte<EEquipmentSlot> toSlot)
{
	if (itemToEquip != nullptr)
	{
		/** Place it on the Equipment Map **/
		EquipmentMap.Emplace(toSlot, itemToEquip);
		/** Apply unique OnEquip rules (Blueprint Script) **/
		itemToEquip->OnEquip();
		
		/** Set utility booleans accordingly **/
		switch (toSlot)
		{
		case EEquipmentSlot::EquipmentSlot_MainHand:
			bHasMainHand = true;
			UpdateMainAttackBonus();
			UpdateMainDamageBonus();
			break;
		case EEquipmentSlot::EquipmentSlot_Offhand:
			if (itemToEquip->ItemType == EItemType::ItemType_Shield)
			{
				bHasShieldEquipped = true;
			}
			bHasOffhand = true;
			break;
		}

		/** Remove item from the inventory **/
		RemoveItemFromInventory(itemToEquip);
		
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - EquipItem: %s equipped."), *itemToEquip->ItemName);
		return true;
	}
	return false;
}

bool UCharacterInventoryComponent::ResolveEquipItem(UBaseEquippableItem* itemToEquip)
{
	bool result = false;
	/** If item type is ARMOR **/
	if (itemToEquip->ItemType == EItemType::ItemType_Armor)
	{
		result = ResolveArmorEquipItem(itemToEquip);
		Owner->OnStatChange();
		return result;
	}
	/** If item type is WEAPON **/
	else if (itemToEquip->ItemType == EItemType::ItemType_Weapon)
	{
		result = ResolveWeaponEquipItem(itemToEquip);
		Owner->OnStatChange();
		return result;
	}
	else if (itemToEquip->ItemType == EItemType::ItemType_Shield)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - ResolveEquipItem: Shield Equip is not implemented."));
	}
	return false;
}

bool UCharacterInventoryComponent::ResolveShieldEquipItem(UBaseEquippableItem* shieldToEquip)
{
	return false;
}

bool UCharacterInventoryComponent::ResolveWeaponEquipItem(UBaseEquippableItem* weaponToEquip)
{
	UBaseWeaponItem* itemAsWeapon = Cast<UBaseWeaponItem>(weaponToEquip);
	if (itemAsWeapon != nullptr)
	{
		/** If there's nothing equipped **/
		if (!bHasMainHand && !bHasOffhand)
		{
			EquipItem(itemAsWeapon, EEquipmentSlot::EquipmentSlot_MainHand);
			return true;
		}
		/** If only mainhand is equipped **/
		else if (bHasMainHand && !bHasOffhand)
		{

		}
		/** If only offhand is equipped **/
		else if (!bHasMainHand && bHasOffhand)
		{

		}
	}
	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - ResolveWeaponItem: itemAsWeapon is nullptr."));
	return false;
}

bool UCharacterInventoryComponent::ResolveArmorEquipItem(UBaseEquippableItem* armorToEquip)
{
	UBaseArmorItem* itemAsArmor = Cast<UBaseArmorItem>(armorToEquip);
	if (itemAsArmor != nullptr)
	{
		UBaseArmorItem* EquippedArmor = Cast<UBaseArmorItem>(GetEquipment(EEquipmentSlot::EquipmentSlot_Armor));
		
		/** If there's already an armor equipped we unequip it **/
		if (EquippedArmor != nullptr)
		{
			BucketEquippable = EquippedArmor;
			UnequipItemFromSlot(EquippedArmor, EEquipmentSlot::EquipmentSlot_Armor); // This function adds the item to the inventory and calls it's unequip rules.
		}
		
		/** Equip the item **/
		EquipItem(armorToEquip, EEquipmentSlot::EquipmentSlot_Armor);

		UpdateArmorClass();

		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - ResolveArmorItem: itemAsArmor is nullptr."));
	return false;
}

bool UCharacterInventoryComponent::EquipItemToSlot(UBaseEquippableItem* itemToEquip, TEnumAsByte<EEquipmentSlot> toSlot)
{
	/** Todo **/
	return false;
}

bool UCharacterInventoryComponent::UnequipItemFromSlot(UBaseEquippableItem* itemToUnequip, TEnumAsByte<EEquipmentSlot> fromSlot)
{
	if (itemToUnequip != nullptr)
	{
		/** Add equipped item to inventory **/
		if (AddItemToInventory(itemToUnequip))
		{
			/** Remove unique item rules **/
			itemToUnequip->OnUnEquip();
			EquipmentMap.Emplace(fromSlot, nullptr);
			switch (fromSlot)
			{
			case EEquipmentSlot::EquipmentSlot_MainHand:
				bHasMainHand = false;
				UpdateMainAttackBonus();
				UpdateMainDamageBonus();
			case EEquipmentSlot::EquipmentSlot_Offhand:
				bHasOffhand = false;
				bHasShieldEquipped = false;
			default:
				break;
			}

			/** Log **/
			UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - UnequipItemFromSlot: %s was successfully unequipped."), *itemToUnequip->ItemName);
			return true;
		}
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - UnequipItemFromSlot: Could not unequip item because AddItemToInventory failed."));
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - UnequipItemFromSlot: Argument item was nullptr."));
	return false;
}

bool UCharacterInventoryComponent::RemoveItemFromInventory(UBaseItem* itemToRemove)
{
	if (itemToRemove != nullptr)
	{
		for (int32 i = 0; i < Inventory.Num(); i++)
		{
			if (Inventory[i] == itemToRemove)
			{
				Inventory[i] = nullptr;
				UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - RemoveItemFromInventory: %s was removed from the Inventory at %i."), *itemToRemove->ItemName,i);
				return true;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - RemoveItemFromInventory: itemToRemove was not found in Inventory."));
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - RemoveItemFromInventory: Argument item was nullptr."));
	return false;
}

bool UCharacterInventoryComponent::RemoveItemFromInventoryByIndex(int32 index)
{
	if (Inventory[index] != nullptr)
	{
		Inventory[index] = nullptr;
		return true;
	}
	return false;
}

bool UCharacterInventoryComponent::AddItemToInventory(UBaseItem* itemToAdd)
{
	if (itemToAdd != nullptr)
	{
		/** Get first null index and place the item in its place **/
		int32 firstNullIndex = GetFirstNullIndex();

		if (firstNullIndex != -1 && firstNullIndex < Inventory.Num())
		{
			Inventory[firstNullIndex] = itemToAdd;
			UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - AddItemToInventory(): %s was added to inventory at %i"),*itemToAdd->ItemName,firstNullIndex);
			return true;
		}
		/** If the inventory is full or the returned index is out of range then the addition is unsuccessful **/
		UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - AddItemToInventory: FirstNullIndex was out of range or -1"));
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("CharacterInventory - AddItemToInventory: Argument item was nullptr"));
	return false;
}

bool UCharacterInventoryComponent::SwapEquippedWithInventory(UBaseEquippableItem* equippedItem, UBaseEquippableItem* itemToEquip)
{
	return false;
}

bool UCharacterInventoryComponent::AddItemFromItemStorage(FString itemCode)
{
	return false;
}

UBaseWeaponItem* UCharacterInventoryComponent::GetMainHandWeapon()
{
	return Cast<UBaseWeaponItem>(*EquipmentMap.Find(EEquipmentSlot::EquipmentSlot_MainHand));
}

UBaseEquippableItem* UCharacterInventoryComponent::GetOffhandWeapon()
{
	return Cast<UBaseWeaponItem>(*EquipmentMap.Find(EEquipmentSlot::EquipmentSlot_Offhand));
}


