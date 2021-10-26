// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "BaseSingleTargetAbility.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API UBaseSingleTargetAbility : public UBaseAbility
{
	GENERATED_BODY()

		UBaseSingleTargetAbility();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbilityDetails)
		TEnumAsByte<ESingleTargetType> SingleTargetType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilityDetails )
	class ABaseCharacter* Target;

	virtual void Execute() override;
};
