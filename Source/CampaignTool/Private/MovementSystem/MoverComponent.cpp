// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementSystem/MoverComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/AiCharacter.h"
#include "MovementSystem/PathfinderComponent.h"
#include "GridSystem/Grid.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

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
		if (MovementSpline->GetNumberOfSplinePoints() > 1)
		{
			CleanupSplineMesh();
			OwnerCharacter->CleanupPathfinding();
			Timeline->PlayFromStart();
		}
	}
	else
	{
		if (!Timeline)
		{
			UE_LOG(LogTemp, Warning, TEXT("Mover::MoveCharacter() - Timeline is nullptr"));
		}
		if (!MovementSpline)
		{
			UE_LOG(LogTemp, Warning, TEXT("Mover::MoveCharacter() - MovementSpline is nullptr"));
		}
		if (!TimelineCurve)
		{
			UE_LOG(LogTemp, Warning, TEXT("Mover::MoveCharacter() - TimelineCurve is nullptr"));
		}
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
		//UE_LOG(LogTemp, Warning, TEXT("NewLocation: %s"), *NewLocation.ToString());
	}
}

void UMoverComponent::OnTimelineFinished()
{
	Timeline->Stop();
	if (OwnerCharacter->CurrentSpeed >= 1.0f)
	{
		OwnerCharacter->bCanMove = true;
		OwnerCharacter->bCanAct = true;
		OwnerCharacter->RefreshPathfinding();
	}
	MovementSpline->ClearSplinePoints();

	if (!OwnerCharacter->bIsPlayerCharacter)
	{
		AAiCharacter* OwnerAsAiCharacter = Cast<AAiCharacter>(OwnerCharacter);
		OwnerAsAiCharacter->ResolveAction();

	}
	UE_LOG(LogTemp, Warning, TEXT("Movement is finished."));
}

void UMoverComponent::RefreshSpline()
{
	if (MovementSpline && OwnerCharacter && OwnerCharacter->Pathfinder && OwnerCharacter->Grid)
	{
		if (MovementSpline->GetNumberOfSplinePoints() > 0)
		{
			MovementSpline->ClearSplinePoints();
		}
		for (int32 i = OwnerCharacter->Pathfinder->Route.Num()-1; i >= 0; i--)
		{
			FVector splinePointLocation;
			int32 Row = OwnerCharacter->Pathfinder->Route[i] / OwnerCharacter->Grid->Columns;
			int32 Column = OwnerCharacter->Pathfinder->Route[i] % OwnerCharacter->Grid->Columns;

			splinePointLocation = FVector((Row * OwnerCharacter->Grid->fieldSize) + (OwnerCharacter->Grid->fieldSize / 2), (Column * OwnerCharacter->Grid->fieldSize) + (OwnerCharacter->Grid->fieldSize / 2), 50.0f);
			MovementSpline->AddSplinePoint(splinePointLocation, ESplineCoordinateSpace::World, true);
			UE_LOG(LogTemp, Warning, TEXT("Mover - RefreshSpline(): Adding SplinePoint to: %s"), *splinePointLocation.ToString());
			
			MovementSpline->SetSplinePointType(i, ESplinePointType::Linear); //Make tangents 0,0,0 vectors TODO
			MovementSpline->SetTangentsAtSplinePoint(i, FVector(0, 0, 0), FVector(0, 0, 0), ESplineCoordinateSpace::Local);
		}
		if (OwnerCharacter->bIsPlayerCharacter)
		{
			RefreshSplineMesh();
		}
		//UE_LOG(LogTemp, Warning, TEXT("SplineMeshes: %i"),MovementSplineMeshArray.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mover - RefreshSpline(): nullptr received"));

	}
}

void UMoverComponent::RefreshSplineMesh()
{
	if (MovementSpline)
	{
		if (MovementSplineMeshArray.Num()>0)
		{
			for (int32 i = 0; i < MovementSplineMeshArray.Num(); i++)
			{
				MovementSplineMeshArray[i]->DestroyComponent();
			}
			MovementSplineMeshArray.Empty();
		}
		for (int32 i = 0; i < MovementSpline->GetNumberOfSplinePoints()-1; i++)
		{
			USplineMeshComponent* newSplineMesh = NewObject<USplineMeshComponent>(this,SplineMesh);
			newSplineMesh->RegisterComponentWithWorld(OwnerCharacter->GetWorld());
			FVector startPos = MovementSpline->GetLocationAtSplinePoint(i,ESplineCoordinateSpace::World);
			startPos.Z = 20.0f;
			//FVector startTan = MovementSpline->GetTangentAtSplinePoint(i,ESplineCoordinateSpace::World);
			FVector endPos = MovementSpline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);
			endPos.Z = 20.0f;
			//FVector endTan = MovementSpline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World);

			newSplineMesh->SetStartAndEnd(startPos, FVector(0,0,0), endPos, FVector(0,0,0));
			MovementSplineMeshArray.Add(newSplineMesh);
		}
	}
}

void UMoverComponent::CleanupSplineMesh()
{
	if (MovementSplineMeshArray.Num() > 0)
	{
		for (int32 i = 0; i < MovementSplineMeshArray.Num(); i++)
		{
			MovementSplineMeshArray[i]->DestroyComponent();
		}
		MovementSplineMeshArray.Empty();
	}
}




// Called every frame
void UMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

