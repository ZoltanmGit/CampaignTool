// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterPawn.generated.h"

UENUM(BlueprintType)
enum CursorState
{
	OverGameWorld UMETA(DisplayName = "OverGameWorld"),
	OverInterface UMETA(DisplayName = "OverInterface"),
	Turning UMETA(DisplayName = "Turning"),
	OverEnemy UMETA(DisplayName = "OverEnemy"),
	OverAlly UMETA(DisplayName = "OverAlly")
};

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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
		class UHealthComponent* CharacterHealth; //Component that manages the health of the character

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		class UStaticMeshComponent* CharacterMesh; //The Mesh that represents the character in-game

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CharacterSpringArm; //The springArm the camera is attached to

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* CharacterCamera; //The Camera the player sees

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controller)
		class APlayerController* DefaultController; //The Default playerController

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TEnumAsByte<CursorState> PlayerCursorState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector PawnLocation; // The location the pawn is supposed to be at

protected:
	//values
	
	FVector2D savedMousePosition; // We save the mouse location and reset it when turning
	bool bIsRightMouseDown;
	bool bCastMouseLineTrace;

	class UHierarchicalInstancedStaticMeshComponent* TargetedTile;
	FHitResult UnderCursorHitResult;
protected:
	//Damage Handling
	UFUNCTION()
		void HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	//Events
	UFUNCTION(BlueprintNativeEvent)
		void OnHealthChange();
	//Test method
	void HandleTestAction();

	//CameraMovement
	void CameraForward(float value);
	void CameraRight(float value);
	void CameraTurn(float value);
	//Mouse Actions
	void HandleRMBPress();
	void HandleRMBRelease();

	void HandleLMBPress();

public:
	FVector GetPawnLocation();

};
