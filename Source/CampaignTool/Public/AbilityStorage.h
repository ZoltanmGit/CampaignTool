// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityStorage.generated.h"

UCLASS()
class CAMPAIGNTOOL_API AAbilityStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityStorage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
		TMap<FString,TSubclassOf<class UBaseAbility>> AbilityMap;
	class UBaseAbility* GetAbilityPtr(FString AbilityCode);
};
