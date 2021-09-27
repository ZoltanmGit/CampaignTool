// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMapMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../Public/MapSaveObject.h"
#include "../Public/PersistenceSaveObject.h"

ANewMapMenuGameMode::ANewMapMenuGameMode()
{
	Rows = 1;
	Columns = 1;
	SelectedTile = -1;
	SelectedObject = -1;
	MapName = "";
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
			int32 delta = NewColumn-Columns;
			for (int32 i = 0; i < delta * Rows; i++)
			{
				TileMatrix.Add(-1);
				ObjectMatrix.Add(-1);
			}
			for (int i = Rows * NewColumn-1; i >= 0; i--)
			{
				TileMatrix[i] = TileMatrix[i - ((i / NewColumn) * delta)];
				if (i % NewColumn > Columns - 1)
				{
					TileMatrix[i] = -1;
				}
			}
		}
		else // If smaller we rearrange values and shrink
		{
			int32 delta = Columns - NewColumn;
			for (int i = 0; i < Rows*NewColumn; i++)
			{
				TileMatrix[i] = TileMatrix[i + ((i / NewColumn) * delta)];
			}
			for (int32 i = 0; i < delta*Rows; i++)
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
		UPersistenceSaveObject* Persist = Cast<UPersistenceSaveObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PersistentDataSlot"), 0));
		if (!Persist)
		{
			Persist = Cast<UPersistenceSaveObject>(UGameplayStatics::CreateSaveGameObject(UPersistenceSaveObject::StaticClass())); // If there is no Persistence yet then we create one
			UE_LOG(LogTemp, Warning, TEXT("Created new persistence"));
		}

		//Save the persistent values of a map
		SaveObject->TileMatrix = TileMatrix;
		SaveObject->MapName = MapName;
		SaveObject->Rows = Rows;
		SaveObject->Columns = Columns;

		
		FString SlotName = "MapSave"+FString::FromInt(Persist->SavedMapNum+1);

		if (UGameplayStatics::SaveGameToSlot(SaveObject, SlotName, 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Saved the map to %s"), *SlotName);
		}
		Persist->SavedMapNum++;
		if (UGameplayStatics::SaveGameToSlot(Persist, TEXT("PersistentDataSlot"), 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Saved Persistence"));
		}
	}
}
void ANewMapMenuGameMode::OnGridChange_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGridChange()"));
}
