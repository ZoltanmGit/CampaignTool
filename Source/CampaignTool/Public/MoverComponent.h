// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "MoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMPAIGNTOOL_API UMoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoverComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MovementParameters)
		float MovementDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MovementParameters)
		class UTimelineComponent* Timeline;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MovementParameters)
		UCurveFloat* TimelineCurve;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MovementParamteres)
		class ABaseCharacter* OwnerCharacter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Spline)
		class USplineComponent* MovementSpline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Spline)
		TSubclassOf<class USplineMeshComponent> SplineMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spline)
		TArray<class USplineMeshComponent*> MovementSplineMeshArray;

	UFUNCTION(BlueprintCallable)
		void MoveCharacter(FVector newLocation);
	UFUNCTION()
		void TimelineFloatReturn(float value);
	UFUNCTION()
		void OnTimelineFinished();
	UFUNCTION()
		void RefreshSpline();
	UFUNCTION()
		void RefreshSplineMesh();
	UFUNCTION()
		void CleanupSplineMesh();

protected:
	FOnTimelineFloat InterpFunction{}; //Delegate for timeline update
	FOnTimelineEvent TimelineFinished{}; //Delegate for timeline finished
};
