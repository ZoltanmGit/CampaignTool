// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverComponent.h"
#include "../Public/BaseCharacter.h"
#include "Components/SplineComponent.h"

// Sets default values for this component's properties
UMoverComponent::UMoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MovementDuration = 2.0f;
	OwnerCharacter = nullptr;

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	Timeline->SetPlayRate(MovementDuration);
	Timeline->SetLooping(false);

	InterpFunction.BindUFunction(this, FName{ TEXT("TimelineFloatReturn") });
	TimelineFinished.BindUFunction(this, FName{ TEXT("OnTimelineFinished") });

	MovementSpline = CreateDefaultSubobject<USplineComponent>(TEXT("MovementSpline"));
}


// Called when the game starts
void UMoverComponent::BeginPlay()
{
	Super::BeginPlay();

	if (TimelineCurve != nullptr)
	{
		Timeline->AddInterpFloat(TimelineCurve, InterpFunction, FName{ TEXT("Alpha") }); // Connect the delegate to the curve
		Timeline->SetTimelineFinishedFunc(TimelineFinished); // Connect the finish function

		OwnerCharacter = Cast<ABaseCharacter>(this->GetOwner());

		MovementSpline->ClearSplinePoints();
	}
}

void UMoverComponent::MoveCharacter(FVector newLocation)
{
	if (Timeline && MovementSpline && TimelineCurve)
	{
		FVector halfvector = (newLocation - OwnerCharacter->CharacterLocation) / 2 + OwnerCharacter->CharacterLocation;
		halfvector.Z = 140.0f;

		MovementSpline->AddSplinePoint(OwnerCharacter->CharacterLocation, ESplineCoordinateSpace::World);
		MovementSpline->AddSplinePoint(halfvector, ESplineCoordinateSpace::World);
		MovementSpline->AddSplinePoint(newLocation, ESplineCoordinateSpace::World);

		FTransform tempTransform;
		tempTransform.SetLocation(OwnerCharacter->CharacterLocation);
		FTransform tempTransform1;
		tempTransform1.SetLocation(halfvector);
		FTransform tempTransform2;
		tempTransform2.SetLocation(newLocation);
		OwnerCharacter->OnPathfinding(tempTransform);
		OwnerCharacter->OnPathfinding(tempTransform1);
		OwnerCharacter->OnPathfinding(tempTransform2);


		OwnerCharacter->CleanupPathfinding();
		Timeline->PlayFromStart();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mover: Cannot move character"));
	}
}

void UMoverComponent::TimelineFloatReturn(float value)
{
	if (Timeline && MovementSpline && TimelineCurve)
	{
		FVector NewLocation = MovementSpline->GetLocationAtDistanceAlongSpline(FMath::Lerp(0.0f, MovementSpline->GetSplineLength(), value), ESplineCoordinateSpace::World);
		NewLocation.X = FMath::Clamp(NewLocation.X, 0.0f, 5000.0f);
		NewLocation.Y = FMath::Clamp(NewLocation.Y, 0.0f, 5000.0f);
		NewLocation.Z = FMath::Clamp(NewLocation.Z, 0.0f, 200.0f);

		OwnerCharacter->SetActorLocation(NewLocation);
		UE_LOG(LogTemp, Warning, TEXT("NewLocation: %s"), *NewLocation.ToString());
	}
}

void UMoverComponent::OnTimelineFinished()
{
	Timeline->Stop();
	if (OwnerCharacter->CurrentSpeed >= 1.0f)
	{
		OwnerCharacter->bCanMove = true;
		OwnerCharacter->RefreshPathfinding();
	}
	MovementSpline->ClearSplinePoints();
	UE_LOG(LogTemp, Warning, TEXT("Movement is finished."));
}


// Called every frame
void UMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

