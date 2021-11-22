// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "BaseAoeTargetAbility.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CAMPAIGNTOOL_API UBaseAoeTargetAbility : public UBaseAbility
{
	GENERATED_BODY()


	UBaseAoeTargetAbility(); //Constructor
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpellDetails)
		TEnumAsByte<EAreaOfEffectType> AreaEffectType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpellDetails)
		TEnumAsByte<EOriginType> OriginType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpellDetails)
		TArray<class ABaseCharacter*> AffectedCharacters;

	virtual void Execute() override;
};
