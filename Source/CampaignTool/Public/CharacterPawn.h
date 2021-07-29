// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterPawn.generated.h"

UCLASS()
class CAMPAIGNTOOL_API ACharacterPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACharacterPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Display)
		class UWidgetComponent* CharacterDisplay;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
		class UHealthComponent* CharacterHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
		class UStaticMeshComponent* CharacterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CharacterSpringArm;

protected:
	//Damage Handling
	UFUNCTION()
		void HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	//Events
	UFUNCTION(BlueprintNativeEvent)
		void OnHealthChange();
	//Test method
	void HandleTestAction();
};
