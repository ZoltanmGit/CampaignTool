// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "AiCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CAMPAIGNTOOL_API AAiCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

		virtual void BeginTurn() override;
		virtual void EndTurn() override;
};
