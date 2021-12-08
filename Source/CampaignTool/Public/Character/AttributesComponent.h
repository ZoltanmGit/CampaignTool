// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStruct.h"
#include "Utilities/AttributeEnums.h"
#include "AttributesComponent.generated.h"

USTRUCT()
struct FSavingThrow
{
	GENERATED_BODY()
public:
	TEnumAsByte<EAbilityType> Ability;
	bool bHasAdvantage;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMPAIGNTOOL_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		FCharacterStruct Stats;
		
public:
	UFUNCTION(BlueprintCallable)
		int32 GetModifier(TEnumAsByte<EAbilityType> Ability);
	UFUNCTION(BlueprintCallable)
		int32 GetSavingThrowModifier(TEnumAsByte<EAbilityType> Ability);
	UFUNCTION(BlueprintCallable)
		int32 GetProficiencyBonus();

	bool IsProficientWith(TEnumAsByte<EWeapon> weapon);
	bool IsProficientWith(TEnumAsByte<EArmor> armor);
	bool IsProficientWith(TEnumAsByte<ETool> tool);
	bool IsImmuneTo(TEnumAsByte<EDamageType> damageType);
	bool IsImmuneTo(TEnumAsByte<ECondition> condition);
	bool IsResistantTo(TEnumAsByte<EDamageType> damageType);


	void InitComponent(FCharacterStruct character);
};
