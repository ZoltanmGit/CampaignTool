// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterStruct.h"
#include "Utilities/AttributeEnums.h"
#include "BaseCharacter.generated.h"

UCLASS()
class CAMPAIGNTOOL_API ABaseCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//Public properties exposed to the editor

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
		class UHealthComponent* CharacterHealth; //Component that manages the health of the character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attributes)
		class UAttributesComponent* CharacterAttributes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		class UAbilityComponent* CharacterAbilityComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		class UCharacterInventoryComponent* CharacterInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pathfinding)
		class UPathfinderComponent* Pathfinder;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
		class UMoverComponent* Mover;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		class UStaticMeshComponent* CharacterMesh; //The Mesh that represents the character in-game
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Roller)
		class UDiceRoller* DiceRoller;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector CharacterLocation; // The location the pawn is supposed to be at, not necessarily where he is.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector CursorLocation; // The location of the tile the cursor is on
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TEnumAsByte<ECharacterType> CharacterType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pathfinding)
		float CurrentSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid)
		class AGrid* Grid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Indicator)
		class AIndicatorActor* Indicator;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
		TArray<class UBaseAbility*> AbilityArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
		TArray<class UBaseAbility*> AcquiredAbilityArray;
	
	// Action Economy
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		int32 Initiative;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 SpellSlotNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bMovementAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bBonusAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bFreeAction;
	//Properties not exposed to the editor
	bool bCanMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bCanAct;
	bool bIsActive;
	bool bIsAimingAbility;
	bool bIsPlayerCharacter;
	bool bIsAlive;
	UFUNCTION(BlueprintCallable)
		void OnCharacterDeath();
	UFUNCTION(BlueprintCallable)
		void OnEnemyDeath();
	UFUNCTION(BlueprintCallable)
		bool IsFlankedByPlayers();

	UFUNCTION(BlueprintCallable)
		virtual void BeginTurn();
	UFUNCTION(BlueprintCallable)
		virtual void EndTurn();
	UFUNCTION()
		void ChangeLocation(FVector newLocation);
	UFUNCTION()
		void RefreshPathfinding();
	UFUNCTION()
		void InitializeCharacter(FCharacterStruct Character, class AGrid* ArgGrid, class AIndicatorActor* ArgIndicator, class AAbilityStorage* ArgAbilityStorage, class AItemStorage* ArgItemStorage);
	UFUNCTION()
		void InitializeEnemyCharacter(class AGrid* ArgGrid, class AIndicatorActor* ArgIndicator, class AAbilityStorage* ArgAbilityStorage, class AItemStorage* ArgItemStorage);
	
	/** Utility Functions **/
	UFUNCTION(BlueprintCallable)
		int32 GetRow() const;
	UFUNCTION(BlueprintCallable)
		int32 GetColumn() const;
	UFUNCTION(BlueprintCallable)
		bool MakeSave(TEnumAsByte<EAbilityType> Ability, int32 difficultyClass) const;
public:

	//Getters
	UFUNCTION(BlueprintCallable)
		class UHealthComponent* GetCharacterHealth() const;
	UFUNCTION(BlueprintCallable)
		class UAttributesComponent* GetCharacterAttributes() const;

	//Public Events
	UFUNCTION(BlueprintNativeEvent)
		void OnHealthChange(); //Event that notifies the Widget to reflect value changes
	UFUNCTION(BlueprintNativeEvent)
		void OnStatChange(); //Event that notifies the Widget to reflect value changes

	UFUNCTION(BlueprintNativeEvent)
		void OnPathfinding(const FTransform transform);
	UFUNCTION(BlueprintNativeEvent)
		void CleanupPathfinding();
	UFUNCTION(BlueprintNativeEvent)
		void OnAbilityAim(const FTransform transform);
	UFUNCTION(BlueprintNativeEvent)
		void CleanupAbilityIndicators();
	UFUNCTION(BlueprintNativeEvent)
		void OnConstructSpellBook();
	UFUNCTION(BlueprintNativeEvent)
		void OnAttackEnemy(const ABaseCharacter* attackedCharacter);
	UFUNCTION(BlueprintNativeEvent)
		void OnBeingAttacked(const ABaseCharacter* attackingCharacter);
	UFUNCTION(BlueprintNativeEvent)
		void OnDamaged();
};
