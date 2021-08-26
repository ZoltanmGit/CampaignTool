// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "EngineUtils.h"
#include "../Public/BaseCharacter.h"
#include "../Public/PathfinderComponent.h"

// Sets default values
AMovementController::AMovementController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = MovementRootComponent;

	MovementDuration = 2.0f;

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	Timeline->SetPlayRate(MovementDuration);
	Timeline->SetLooping(false);

	InterpFunction.BindUFunction(this, FName{ TEXT("TimelineFloatReturn") });
	TimelineFinished.BindUFunction(this, FName{ TEXT("OnTimelineFinished") });
}

// Called when the game starts or when spawned
void AMovementController::BeginPlay()
{
	Super::BeginPlay();
	/*if (TimelineCurve != nullptr)
	{
		Timeline->AddInterpFloat(TimelineCurve, InterpFunction, FName{ TEXT("Alpha") }); // Connect the delegate to the curve
		Timeline->SetTimelineFinishedFunc(TimelineFinished); // Connect the finish function
	}*/
}

// Called every frame
void AMovementController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*for (int32 i = 0; i < Characters.Num(); i++)
	{
		/*if (Characters[i])
		{
			/*if (Characters[i]->GetActorLocation() != Characters[i]->CharacterLocation)
			{
				if (Characters[i]->bCanMove)
				{
					Characters[i]->bCanMove = false;
				}
				//Characters[i]->SetActorLocation(Characters[i]->MovementSpline->FindLocationClosestToWorldLocation(FMath::VInterpTo(Characters[i]->GetActorLocation(), Characters[i]->CharacterLocation, DeltaTime, 10.0f), ESplineCoordinateSpace::World));
				Characters[i]->SetActorLocation(FVector(FMath::VInterpTo(Characters[i]->GetActorLocation(), Characters[i]->CharacterLocation, DeltaTime, 10.0f).X, FMath::VInterpTo(Characters[i]->GetActorLocation(), Characters[i]->CharacterLocation, DeltaTime, 10.0f).Y, 50.0f));
			}
			else if (Characters[i]->bIsCharacterActive && Characters[i]->CurrentSpeed >= 1.0f && !Characters[i]->bCanMove)
			{
				Characters[i]->bCanMove = true;
				Characters[i]->RefreshPathfinding();
			}
		}
		if (Characters[i])
		{
			if (Characters[i]->bIsCharacterActive && Characters[i]->bCanMove && Characters[i]->GetActorLocation() != Characters[i]->CharacterLocation)
			{
				Characters[i]->bCanMove = false;
				MoveCharacter(Characters[i]);
			}
		}
	}*/
		
}

void AMovementController::SubscribeToMovement(ABaseCharacter* character)
{
	Characters.Add(character);
}


void AMovementController::MoveCharacter(ABaseCharacter* charToMove)
{
	UE_LOG(LogTemp, Warning, TEXT("Movement Started"));
	Timeline->Play();
}

void AMovementController::TimelineFloatReturn(float value, ABaseCharacter* MovingCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("TimelineFloat is : %f"), value);
}
void AMovementController::OnTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Movement Finished"));
}

