// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovementController.generated.h"

UCLASS()
class CAMPAIGNTOOL_API AMovementController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovementController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MovementParameters)
		float MovementDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Root)
		class USceneComponent* MovementRootComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
		class USplineComponent* MovementSpline;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Characters)
		TArray<class ABaseCharacter*> Characters;

	UFUNCTION(BlueprintCallable)
		void SubscribeToMovement(class ABaseCharacter* character);

};
