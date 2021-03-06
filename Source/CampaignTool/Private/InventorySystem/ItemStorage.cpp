// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemStorage.h"
#include "InventorySystem/BaseItem.h"

// Sets default values
AItemStorage::AItemStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AItemStorage::BeginPlay()
{
	Super::BeginPlay();
}

UBaseItem* AItemStorage::GetItemPtr(FString itemCode)
{
	TSubclassOf<UBaseItem> itemTemplate = nullptr;
	switch (itemCode[0])
	{
	case 'a':
		itemTemplate = ArmorMap.Find(itemCode)->Get();
		break;
	case 'w':
		itemTemplate = WeaponMap.Find(itemCode)->Get();
		break;
	case 'c':
		itemTemplate = ArmorMap.Find(itemCode)->Get();
		break;
	default:
		break;
	}
	if (itemTemplate != nullptr)
	{
		return NewObject<UBaseItem>(this, itemTemplate);
	}
	return nullptr;
}

