// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
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
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CharacterSpringArm; //The springArm the camera is attached to

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* CharacterCamera; //The Camera the player sees

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controller)
		class APlayerController* DefaultController; //The Default playerController

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TEnumAsByte<CursorState> PlayerCursorState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ABaseCharacter* TargetedCharacter;
protected:
	//values

	FVector2D savedMousePosition; // We save the mouse location and reset it when turning
	bool bIsRightMouseDown;
	bool bCastMouseLineTrace;

	FHitResult UnderCursorHitResult;
	UPROPERTY()
		class UHierarchicalInstancedStaticMeshComponent* TargetedTile;

	//Test method
	void HandleTestAction();
	void HandleHotkey(int index);

	//CameraMovement
	void CameraForward(float value);
	void CameraRight(float value);
	void CameraTurn(float value);
	//Mouse Actions
	void HandleRMBPress();
	void HandleRMBRelease();

	void HandleLMBPress();
	void ChangePossession(class ABaseCharacter* newCharacter);

	/**REDUNDANT FOR DEBUG PURPOSES**/
	UPROPERTY(VisibleAnywhere)
		class UBaseAoeTargetAbility* TestAbility01;
	UPROPERTY(VisibleAnywhere)
		class UBaseAoeTargetAbility* TestAbility02;
	UPROPERTY(VisibleAnywhere)
		class UBaseAoeTargetAbility* TestAbility03;
	/** For parametered bindaction use **/
	DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const int);
public:
	
};
