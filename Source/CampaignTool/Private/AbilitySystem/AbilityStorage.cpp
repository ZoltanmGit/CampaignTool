// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityStorage.h"
#include "AbilitySystem/BaseAbility.h"

// Sets default values
AAbilityStorage::AAbilityStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAbilityStorage::BeginPlay()
{
	Super::BeginPlay();
	
}

UBaseAbility* AAbilityStorage::GetAbilityPtr(FString AbilityCode)
{
	if (AbilityMap.Contains(AbilityCode))
	{
		UBaseAbility* AbilityPtr = NewObject<UBaseAbility>(this,AbilityMap.Find(AbilityCode)->Get());
		UE_LOG(LogTemp, Warning, TEXT("GetAbilityPtr"));
		return AbilityPtr;
	}
	return nullptr;
}

