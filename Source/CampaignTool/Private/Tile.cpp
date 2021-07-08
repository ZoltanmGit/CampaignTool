// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATile::ATile()
{
	TileState = TileState::Empty;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

