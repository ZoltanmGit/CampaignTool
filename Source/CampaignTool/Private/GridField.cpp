// Fill out your copyright notice in the Description page of Project Settings.


#include "GridField.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AGridField::AGridField()
{
	FieldCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	FieldCollisionBox->SetBoxExtent(FVector(100.0f, 100.0f, 20.0f));
	RootComponent = FieldCollisionBox;
	FieldState = FieldState::Empty;
}

// Called when the game starts or when spawned
void AGridField::BeginPlay()
{
	Super::BeginPlay();
}
