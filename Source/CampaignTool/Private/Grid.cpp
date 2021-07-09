// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "../Public/GridField.h"

// Sets default values
AGrid::AGrid()
{
	Rows = 10;
	Columns = 10;
	fieldSize = 100;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AGrid::SpawnGrid()
{
	int adjustment = fieldSize / 2; //Should be 50

	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			FTransform spawnTransform;
			spawnTransform.SetLocation(FVector(this->GetActorLocation().X + (i * fieldSize) + adjustment, this->GetActorLocation().Y + (j * fieldSize) + adjustment, this->GetActorLocation().Z+5));
			AGridField* newField = GetWorld()->SpawnActor<AGridField>(AGridField::StaticClass(),spawnTransform);
			newField->Row = i;
			newField->Column = j;
			newField->SetActorRelativeLocation(FVector((i * fieldSize) + adjustment, (j * fieldSize) + adjustment, 5));
			newField->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

			//Add it to our array of fields
			Grid.Add(newField);
		}
	}
}
