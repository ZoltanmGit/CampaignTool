// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementController.h"
#include "Kismet/GameplayStatics.h"
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

}

// Called when the game starts or when spawned
void AMovementController::BeginPlay()
{
	Super::BeginPlay();

	/*for (TActorIterator<ABaseCharacter> CharacterItr(GetWorld()); CharacterItr; ++CharacterItr)
	{
		ABaseCharacter* Character = *CharacterItr;
		UE_LOG(LogTemp, Warning, TEXT("Found Character."));
		Characters.Add(Character);
	}*/
}

// Called every frame
void AMovementController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i = 0; i < Characters.Num(); i++)
	{
		if (Characters[i]->GetActorLocation() != Characters[i]->CharacterLocation)
		{
			if (Characters[i]->bCanMove)
			{
				Characters[i]->bCanMove = false;
			}
			Characters[i]->SetActorLocation(FVector(FMath::VInterpTo(Characters[i]->GetActorLocation(), Characters[i]->CharacterLocation, DeltaTime,10.0f).X, FMath::VInterpTo(Characters[i]->GetActorLocation(), Characters[i]->CharacterLocation, DeltaTime, 10.0f).Y, 50.0f));
		}
		else if (Characters[i]->bIsCharacterActive && Characters[i]->CurrentSpeed >= 1.0f && !Characters[i]->bCanMove)
		{
			Characters[i]->bCanMove = true;
			Characters[i]->RefreshPathfinding();
		}
		else
		{
		}
	}
}

void AMovementController::SubscribeToMovement(ABaseCharacter* character)
{
	Characters.Add(character);
}

