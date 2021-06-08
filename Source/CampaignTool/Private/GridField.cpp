// Fill out your copyright notice in the Description page of Project Settings.


#include "GridField.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGridField::AGridField()
{
	FieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FieldMesh"));
	RootComponent = FieldMesh;
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FieldObject"));
	ObjectMesh->SetupAttachment(RootComponent);
	ObjectMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	FieldState = FieldState::Empty;
}

// Called when the game starts or when spawned
void AGridField::BeginPlay()
{
	Super::BeginPlay();
}
void AGridField::ObjectAdded()
{
	//Implementation needed
}
