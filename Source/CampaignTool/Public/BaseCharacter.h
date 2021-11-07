// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterStruct.h"
#include "AttributeEnums.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		class UAttributesComponent* CharacterAttributes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		class UAbilityComponent* CharacterAbilityComponent;
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

	//Properties not exposed to the editor
	bool bCanMove;
	bool bCanAct;
	bool bIsActive;
	bool bIsAimingAbility;
	/// <summary>
	/// Function that handles changes in health, healing and damage both
	/// </summary>
	UFUNCTION()
		void HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	/// <summary>
	/// Initiates pathfinding, resets actions and advances cooldowns
	/// </summary>
	UFUNCTION()
		void BeginTurn();
	/// <summary>
	/// Ends the turn for the character
	/// </summary>
	UFUNCTION(BlueprintCallable)
		void EndTurn();
	/// <summary>
	/// Changes the supposed location of the character, which then gets moved by the MovementController
	/// </summary>
	UFUNCTION()
		/// <summary>
		/// Moves the character to a new location
		/// </summary>
		/// <param name="newLocation">The new location for the character</param>
		void ChangeLocation(FVector newLocation);
	UFUNCTION()
		void RefreshPathfinding();
	UFUNCTION()
		/// <summary>
		/// Initializes the attributes of a character
		/// Passes a pointer of the grid to the character
		/// Tells the pathfinder how large the grid is
		/// </summary>
		/// <param name="Character"></param>
		/// <param name="ArgGrid"></param>
		void InitializeCharacter(FCharacterStruct Character, class AGrid* ArgGrid, class AIndicatorActor* ArgIndicator);
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
		void OnPathfinding(const FTransform transform);
	UFUNCTION(BlueprintNativeEvent)
		void CleanupPathfinding();
	UFUNCTION(BlueprintNativeEvent)
		void OnAbilityAim(const FTransform transform);
	UFUNCTION(BlueprintNativeEvent)
		void CleanupAbilityIndicators();
};
