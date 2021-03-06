// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStorage.generated.h"

UCLASS()
class CAMPAIGNTOOL_API AItemStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemStorage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	
	/** Properties **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
		TMap<FString, TSubclassOf<class UBaseItem>> ArmorMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
		TMap<FString, TSubclassOf<class UBaseItem>> WeaponMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
		TMap<FString, TSubclassOf<class UBaseItem>> ConsumableMap;
	
	/** Functions **/
	class UBaseItem* GetItemPtr(FString itemName);
};
