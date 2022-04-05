// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */

UCLASS()
class CAMPAIGNTOOL_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	
	/** Properties **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Characters)
		TArray<class AAiCharacter*> ControlledCharacters;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Characters)
		int32 CharacterIndex;
	
	UFUNCTION()
		void ResolveCharacterAtIndex();

};
