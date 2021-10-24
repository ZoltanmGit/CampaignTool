// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IndicatorActor.generated.h"

UENUM(BlueprintType)
enum EIndicatorType
{
	MovementIndicator UMETA(DisplayName = "MovementIndicator"),
	AbilityIndicator UMETA(DisplayName = "AbilityIndicator")
};

UCLASS()
class CAMPAIGNTOOL_API AIndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIndicatorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Indicators **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = General)
		class UHierarchicalInstancedStaticMeshComponent* MovementIndicatorHISMC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = General)
		class UHierarchicalInstancedStaticMeshComponent* AbilityIndicatorHISMC;
	/** Particle System **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = General)
		class UParticleSystemComponent* ParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = General)
		class USceneComponent* RootScene;

public:
	UFUNCTION(BlueprintCallable)
		void SpawnIndicator(FVector location, EIndicatorType IndicatorType);
	UFUNCTION(BlueprintCallable)
		void CleanupIndicators(EIndicatorType IndicatorType);
};
