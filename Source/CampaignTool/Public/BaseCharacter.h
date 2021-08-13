// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum CharacterType
{
	Ally,
	Hostile,
	Self
};

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		class UStaticMeshComponent* CharacterMesh; //The Mesh that represents the character in-game

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector CharacterLocation; // The location the pawn is supposed to be at, not necessarily where he is.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector CursorLocation; // The location of the tile the cursor is on
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TEnumAsByte<CharacterType> CharacterType; // The location of the tile the cursor is on

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid)
		class AGrid* Grid;

	//Function that handles changes in health
	UFUNCTION()
		void HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
public:

	//Getters
	UFUNCTION(BlueprintCallable)
		class UHealthComponent* GetCharacterHealth() const;
	UFUNCTION(BlueprintCallable)
		class UAttributesComponent* GetCharacterAttributes() const;

	//Public Events
	UFUNCTION(BlueprintNativeEvent)
		void OnHealthChange(); //Event that notifies the Widget to reflect value changes
};
