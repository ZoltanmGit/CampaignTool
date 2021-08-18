// Copyright Epic Games, Inc. All Rights Reserved.


#include "CampaignToolGameModeBase.h"
#include "Public/MovementController.h"

void ACampaignToolGameModeBase::BeginPlay()
{
	//Spawn Grid
	//Spawn Characters
	//Spawn MovementController
	InitializeMovementController();
}

void ACampaignToolGameModeBase::InitializeGrid()
{
}

void ACampaignToolGameModeBase::InitializeCharacters()
{
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
