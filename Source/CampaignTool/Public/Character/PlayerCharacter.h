// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum CursorState
{
	OverGameWorld UMETA(DisplayName = "OverGameWorld"),
	OverInterface UMETA(DisplayName = "OverInterface"),
	Turning UMETA(DisplayName = "Turning"),
	OverEnemy UMETA(DisplayName = "OverEnemy"),
	OverAlly UMETA(DisplayName = "OverAlly"),
};

UCLASS()
class CAMPAIGNTOOL_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
protected:
	virtual void BeginPlay() override;
public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void EndTurn() override;

	virtual void BeginTurn() override;

	void BeginPreparationTurn();
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CharacterSpringArm; //The springArm the camera is attached to

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* CharacterCamera; //The Camera the player sees

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controller)
		class APlayerController* DefaultController; //The Default playerController

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TEnumAsByte<CursorState> PlayerCursorState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsInventoryCollapsed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ABaseCharacter* TargetedCharacter;

	UFUNCTION(BlueprintCallable)
		void ToogleMovement();

protected:
	FVector2D savedMousePosition; // We save the mouse location and reset it when turning
	bool bIsRightMouseDown;
	bool bCastMouseLineTrace;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsAimingMovement;

	FHitResult UnderCursorHitResult;
	
	UPROPERTY()
		class UHierarchicalInstancedStaticMeshComponent* TargetedTile;

	//Test method
	void HandleTestAction();
	void HandleHotkey(int index);
	UFUNCTION(BlueprintCallable)
		void SelectAbility(class UBaseAbility* abilityToSelect);

	//CameraMovement
	void CameraForward(float value);
	void CameraRight(float value);
	void CameraTurn(float value);
	//Mouse Actions
	void HandleRMBPress();
	void HandleRMBRelease();

	void HandleLMBPress();
	void ChangePossession(class ABaseCharacter* newCharacter);
	bool IsCursorOnValidMovementTile(int32 x, int32 y);

	/** Functions **/
	void HandleInventoryToogle();

	/** For parametered bindaction use **/
	DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const int); /** Used for hotkey input bindings **/

	/**REDUNDANT FOR DEBUG PURPOSES**/
	//Area of Effect
	UPROPERTY(VisibleAnywhere)
		class UBaseAoeTargetAbility* TestAbility01;
	UPROPERTY(VisibleAnywhere)
		class UBaseAoeTargetAbility* TestAbility02;
	UPROPERTY(VisibleAnywhere)
		class UBaseAoeTargetAbility* TestAbility03;
	//Single Target
	UPROPERTY(VisibleAnywhere)
		class UBaseSingleTargetAbility* TestAbility04;
	UPROPERTY(VisibleAnywhere)
		class UBaseSingleTargetAbility* TestAbility05;
	UPROPERTY(VisibleAnywhere)
		class UBaseSingleTargetAbility* TestAbility06;
	
	/** Events **/
	UFUNCTION(BlueprintNativeEvent)
		void OnInventoryChange();
	UFUNCTION(BlueprintNativeEvent)
		void OnInventoryOpen();
	UFUNCTION(BlueprintNativeEvent)
		void OnInventoryClose();
public:
	
};
