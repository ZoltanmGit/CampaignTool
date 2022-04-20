// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "AiCharacter.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EEnemyBehaviour
{
	EUndefinedBehaviour UMETA(DisplayName = "Undefined Behaviour"),
	EMelee UMETA(DisplayName = "EMelee")
};

UCLASS()
class CAMPAIGNTOOL_API AAiCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	virtual void BeginTurn() override;
	virtual void EndTurn() override;

	void ResolveMovement();

	void ResolveAction();

	UFUNCTION(BlueprintNativeEvent)
		void OnMove();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = General)
		FString EnemyName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		ABaseCharacter* EnemyFocus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
		TEnumAsByte<EEnemyBehaviour> Behaviour;

private:
	ABaseCharacter* GetNeighbor();
	FVector GetLocationFromIndex(int32 index);
	int32 GetFurthestOnRouteIndex();
};
