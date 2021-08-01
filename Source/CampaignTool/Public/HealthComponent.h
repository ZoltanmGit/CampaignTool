// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMPAIGNTOOL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//Private values
	float currentHealth;
	float fullHealth;
	float armorClass;
public:
	void HandleTakeDamage(float Damage);
public:
	// Getters, we need to be able to call them from blueprint
	// as to represent them for the User Interface widgets
	UFUNCTION(BlueprintCallable)
		float GetCurrentHealth();
	UFUNCTION(BlueprintCallable)
		float GetFullHealth();
	UFUNCTION(BlueprintCallable)
		float GetArmorClass();
	UFUNCTION(BlueprintCallable)
		float GetHealthPercent();
	//Setters
	void SetCurrentHealth(float value);
	void SetFullHealth(float value);
};
