// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMapMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../Public/MapSaveObject.h"

ANewMapMenuGameMode::ANewMapMenuGameMode()
{
	Rows = 1;
	Columns = 1;
	SelectedTile = -1;
	SelectedObject = -1;
}

void ANewMapMenuGameMode::BeginPlay()
{
	TileMatrix.Add(-1);
	ObjectMatrix.Add(-1);

	SaveObject = Cast<UMapSaveObject>(UGameplayStatics::CreateSaveGameObject(UMapSaveObject::StaticClass()));
}

void ANewMapMenuGameMode::ResizeRow(int32 NewRow)
{
	UE_LOG(LogTemp, Warning, TEXT("RowSize commited"))
	if (NewRow != Rows)
	{
		if (Rows < NewRow) //If larger we extend
		{
			for (int32 i = 0; i < (NewRow-Rows)*Columns; i++)
			{
				TileMatrix.Add(-1);
				ObjectMatrix.Add(-1);
			}
			
		}
		else // If smaller we shrink
		{
			for (int32 i = 0; i < (Rows-NewRow) * Columns; i++)
			{

				TileMatrix.RemoveAt(TileMatrix.Num() - 1);
				ObjectMatrix.RemoveAt(ObjectMatrix.Num() - 1);
			}
		}
		Rows = NewRow;
		OnGridChange();
	}
}

void ANewMapMenuGameMode::ResizeColumn(int32 NewColumn)
{
	UE_LOG(LogTemp, Warning, TEXT("Column size commited"))
	if (NewColumn != Columns)
	{
		
		if (Columns < NewColumn)
		{
			for (int32 i = 0; i < (NewColumn-Columns)*Rows; i++)
			{
				TileMatrix.Add(-1);
				ObjectMatrix.Add(-1);
			}

		}
		else // If smaller we rearrange values and shrink
		{
			for (int32 i = 0; i < Rows*NewColumn; i++)
			{
				TileMatrix[i] = TileMatrix[i+(i/NewColumn)];
				UE_LOG(LogTemp, Warning, TEXT("%i := %i"), i, i + i/NewColumn);
			}
			for (int32 i = 0; i < (Columns-NewColumn)*Rows; i++)
			{
				TileMatrix.RemoveAt(TileMatrix.Num() - 1);
				ObjectMatrix.RemoveAt(ObjectMatrix.Num() - 1);
			}
		}
		Columns = NewColumn;
		OnGridChange();
	}
}
void ANewMapMenuGameMode::SaveMap()
{
	if (SaveObject != nullptr)
	{
		SaveObject->TileMatrix = TileMatrix;
		SaveObject->MapName = TEXT("Placeholdername");
		SaveObject->Rows = Rows;
		SaveObject->Columns = Columns;
		if (UGameplayStatics::SaveGameToSlot(SaveObject, TEXT("TestMap01"), 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Saved the map"));
		}
	}
}
void ANewMapMenuGameMode::OnGridChange_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGridChange()"));
}
