// Copyright Epic Games, Inc. All Rights Reserved.


#include "CampaignToolGameModeBase.h"
#include "Public/MovementController.h"
#include "Public/Grid.h"
#include "Public/PlayerCharacter.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void ACampaignToolGameModeBase::BeginPlay()
{
	UserController = Cast<APlayerController>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerController::StaticClass()));
	if (UserController)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("PlayerController found"));

		//Spawn Grid
		InitializeGrid();
		//Spawn Characters
		InitializeCharacters();
		//Spawn MovementController
		InitializeMovementController();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not found!"));
	}
}

void ACampaignToolGameModeBase::InitializeGrid()
{
	if (Grid)
	{
		FActorSpawnParameters SpawnParams;
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(0.0f, 0.0f, 0.0f));
		AGrid* tempGrid = GetWorld()->SpawnActor<AGrid>(Grid, TransformParams, SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("Grid initialized."));
	}
}

void ACampaignToolGameModeBase::InitializeCharacters()
{
	FActorSpawnParameters SpawnParams;
	if (FighterClass)
	{
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(50.0f, 50.0f, 50.0f));
		APlayerCharacter* newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(FighterClass, TransformParams, SpawnParams);
		newCharacter->DefaultController = UserController;
		UserController->Possess(newCharacter);
		newCharacter->bIsPossessed = true;
		UE_LOG(LogTemp, Warning, TEXT("Fighter spawned. (And possessed)"));
	}
	/*if (RogueClass)
	{
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(150.0f, 50.0f, 50.0f));
		APlayerCharacter* newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RogueClass, TransformParams, SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("Rogue spawned."));
	}
	if (WizardClass)
	{
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(250.0f, 50.0f, 50.0f));
		APlayerCharacter* newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(WizardClass, TransformParams, SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("Wizard spawned."));
	}*/
}

void ACampaignToolGameModeBase::InitializeMovementController()
{
	if (MovementController)
	{
		FActorSpawnParameters SpawnParams;
		FTransform TransformParams;
		TransformParams.SetLocation(FVector(-100.0f, -100.0f, 0.0f));
		GetWorld()->SpawnActor<AMovementController>(MovementController,TransformParams,SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("MovementController initialized."));
	}
}
