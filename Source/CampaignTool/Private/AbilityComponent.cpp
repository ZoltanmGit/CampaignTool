// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "BaseAbility.h"
#include "BaseCharacter.h"
#include "BaseAoeTargetAbility.h"
#include "BaseCharacter.h"
#include "IndicatorActor.h"
#include "BaseSingleTargetAbility.h"
#include "Grid.h"


// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAbilityComponent::HandleTileChange()
{
	if (SelectedAbility!= nullptr && Owner != nullptr && Owner->Grid != nullptr)
	{
		ResetSelectionData();
		Owner->CleanupAbilityIndicators();
		if (SelectedAbility->TargetType == ETargetType::AOE)
		{
			UBaseAoeTargetAbility* AbilityAsAOE = Cast<UBaseAoeTargetAbility>(SelectedAbility);
			// Line
			if (AbilityAsAOE != nullptr && AbilityAsAOE->AreaEffectType == EAreaOfEffectType::Line)
			{
				// Character coordinates
				int32 DummyIndex;
				FTileProperties temptileprop = Owner->Grid->GetTilePropertiesFromTransform(Owner->GetActorTransform(), DummyIndex);
				int32 x0 = temptileprop.Row;
				int32 y0 = temptileprop.Column;

				// Cursor coordinates
				FTransform CursorTransform;
				CursorTransform.SetLocation(Owner->CursorLocation);
				temptileprop = Owner->Grid->GetTilePropertiesFromTransform(CursorTransform, DummyIndex);
				int32 x1 = temptileprop.Row;
				int32 y1 = temptileprop.Column;

				ResolveLine(x0, y0, x1, y1);
			}
			// Sphere
			else if (AbilityAsAOE != nullptr && AbilityAsAOE->AreaEffectType == EAreaOfEffectType::Sphere)
			{
				// Character coordinates
				int32 DummyIndex;
				FTileProperties temptileprop = Owner->Grid->GetTilePropertiesFromTransform(Owner->GetActorTransform(), DummyIndex);
				int32 x_char = temptileprop.Row;
				int32 y_char = temptileprop.Column;

				// Cursor coordinates
				FTransform CursorTransform;
				CursorTransform.SetLocation(Owner->CursorLocation);
				temptileprop = Owner->Grid->GetTilePropertiesFromTransform(CursorTransform, DummyIndex);
				int32 x_curs = temptileprop.Row;
				int32 y_curs = temptileprop.Column;

				InitRangeGrid(x_char, y_char);
				if (RangeDijkstraGrid[Owner->Grid->CoordToIndex(x_curs, y_curs)].NodeValue <= SelectedAbility->Range)
				{
					ResolveSphere(x_curs, y_curs);
				}
			}
			// Cone
			else if (AbilityAsAOE != nullptr && AbilityAsAOE->AreaEffectType == EAreaOfEffectType::Cone)
			{
				// Character coordinates
				int32 DummyIndex;
				FTileProperties temptileprop = Owner->Grid->GetTilePropertiesFromTransform(Owner->GetActorTransform(), DummyIndex);
				int32 x_char = temptileprop.Row;
				int32 y_char = temptileprop.Column;

				// Cursor coordinates
				FTransform CursorTransform;
				CursorTransform.SetLocation(Owner->CursorLocation);
				temptileprop = Owner->Grid->GetTilePropertiesFromTransform(CursorTransform, DummyIndex);
				int32 x_curs = temptileprop.Row;
				int32 y_curs = temptileprop.Column;

				if (!(x_curs == x_char && y_curs == y_char))
				{
					// Get Direction
					TEnumAsByte<EConeDirection> Direction = GetConeDirection(x_char, y_char, x_curs, y_curs);
					if ( Direction != EConeDirection::D_Undefined)
					{
						ResolveCone(x_char, y_char, Direction);
					}
				}
			}
		}
		else if (SelectedAbility->TargetType == ETargetType::Single)
		{
			int32 DummyIndex;
			FTileProperties temptileprop = Owner->Grid->GetTilePropertiesFromTransform(Owner->GetActorTransform(), DummyIndex);
			int32 x_char = temptileprop.Row;
			int32 y_char = temptileprop.Column;
			InitRangeGrid(x_char, y_char);

			FTransform CursorTransform;
			CursorTransform.SetLocation(Owner->CursorLocation);
			temptileprop = Owner->Grid->GetTilePropertiesFromTransform(CursorTransform, DummyIndex);
			int32 x_curs = temptileprop.Row;
			int32 y_curs = temptileprop.Column;

			UBaseSingleTargetAbility* AbilityAsSingle = Cast<UBaseSingleTargetAbility>(SelectedAbility);
			if (AbilityAsSingle != nullptr && AbilityAsSingle->SingleTargetType == ESingleTargetType::NonAttack)
			{
				ResolveNonAttack(x_curs, y_curs);
			}
			else if (AbilityAsSingle != nullptr && AbilityAsSingle->SingleTargetType == ESingleTargetType::MeleeAttack)
			{
				ResolveMelee(x_curs, y_curs);
			}
			else if (AbilityAsSingle != nullptr && AbilityAsSingle->SingleTargetType == ESingleTargetType::RangedAttack)
			{
				ResolveRanged(x_curs, y_curs);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AbilityAsSingle is nullptr."));
			}
		}
	}
}

void UAbilityComponent::ExecuteAbility()
{
	if (SelectedAbility != nullptr)
	{
		if (SelectedAbility->TargetType == ETargetType::AOE)
		{
			UBaseAoeTargetAbility* AOEAbility = Cast<UBaseAoeTargetAbility>(SelectedAbility);
			AOEAbility->AffectedCharacters = AffectedCharacters;
			AOEAbility->Execute();
			AOEAbility->OnExecute();
		}
		else if (SelectedAbility->TargetType == ETargetType::Single && AffectedCharacters[0] != nullptr)
		{
			UBaseSingleTargetAbility* SingleTargetAbility = Cast<UBaseSingleTargetAbility>(SelectedAbility);
			SingleTargetAbility->TargetCharacter = AffectedCharacters[0];
			SingleTargetAbility->Execute();
			SingleTargetAbility->OnExecute();
		}
		CancelAbility();
		Owner->bIsAimingAbility = false;

	}
}

void UAbilityComponent::SelectAbility(UBaseAbility* AbilityToSelect)
{
	SelectedAbility = AbilityToSelect;
}

void UAbilityComponent::ResolveSphere(int32 x, int32 y)
{
	if (SphereDijkstraGrid.Num() > 0)
	{
		SphereDijkstraGrid.Empty();
	}

	for (int32 i = 0; i < Owner->Grid->Rows; i++)
	{
		for (int32 j = 0; j < Owner->Grid->Columns; j++)
		{
			FDijkstraNode node;
			node.x = i;
			node.y = j;
			node.bWasProcessed = false;
			node.bIsValidTerrain = true;

			if (x == i && y == j)
			{
				node.NodeValue = 0;
			}
			else
			{
				node.NodeValue = Owner->Grid->Rows * Owner->Grid->Columns + 1;
			}
			SphereDijkstraGrid.Add(node);
		}
	}

	TPair<int32, int32> s = TPair<int32, int32>(x, y);
	SphereDijkstraQueue.Enqueue(s);
	while (SphereDijkstraQueue.IsEmpty() == false)
	{
		TPair<int32, int32> pair;
		SphereDijkstraQueue.Dequeue(pair);
		ProcessNodeForSphere(pair.Key, pair.Value, SelectedAbility->OptionalRange);
	}
}

void UAbilityComponent::ResolveLine(int32 x0, int32 y0, int32 x1, int32 y1)
{
	InitRangeGrid(x0, y0);
	if (abs(y1 - y0) < abs(x1 - x0))
	{

		if (x0 > x1)
		{
			PlotTileLow(x1, y1, x0, y0, true);
		}
		else
		{
			PlotTileLow(x0, y0, x1, y1, false);
		}
	}
	else
	{
		if (y0 > y1)
		{
			PlotTileHigh(x1, y1, x0, y0,true);
		}
		else
		{
			PlotTileHigh(x0, y0, x1, y1,false);
		}
	}
}

void UAbilityComponent::ResolveCone(int32 x, int32 y, TEnumAsByte<EConeDirection> Direction)
{
	if (Direction == EConeDirection::D_Up || Direction == EConeDirection::D_Down)
	{
		ResolveConeVertical(x, y, Direction);
	}
	else if (Direction == EConeDirection::D_Left || Direction == EConeDirection::D_Right)
	{
		ResolveConeHorizontal(x, y, Direction);
	}
	else
	{
		switch (Direction)
		{
		case EConeDirection::D_UpLeft:
			if (Owner->Grid->IsValidCoord(x + 1, y - 1))
			{

				ResolveConeDiagonal(x + 1, y - 1, Direction);
			}
			break;
		case EConeDirection::D_UpRight:
			if (Owner->Grid->IsValidCoord(x + 1, y + 1))
			{
				ResolveConeDiagonal(x + 1, y + 1, Direction);
			}
			break;
		case EConeDirection::D_DownLeft:
			if (Owner->Grid->IsValidCoord(x - 1, y - 1))
			{
				ResolveConeDiagonal(x - 1, y - 1, Direction);
			}
			break;
		case EConeDirection::D_DownRight:
			if (Owner->Grid->IsValidCoord(x - 1, y + 1))
			{
				ResolveConeDiagonal(x - 1, y + 1, Direction);
			}
			break;
		default:
			break;
		}
	}
}

void UAbilityComponent::ResolveMelee(int32 x, int32 y)
{
	if (GetRangeValue(x, y) > 0 && GetRangeValue(x, y) < 2  )
	{
		/** CHEKC FOR LINE OF SIGHT **/
		if (Owner->Grid->IsValidCoord(x, y))
		{
			/** Add Tiles **/
			AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

			/** Display Indicators **/
			FTransform transform;
			transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
			Owner->OnAbilityAim(transform);

			/** Add Characters **/
			FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y);
			if (AffectedTile.ActorOnTile != nullptr)
			{
				ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
				if (CharacterToAdd != nullptr)
				{
					AffectedCharacters.Add(CharacterToAdd);
					UE_LOG(LogTemp, Warning, TEXT("Added character"));
				}
			}
		}
	}
}

void UAbilityComponent::ResolveRanged(int32 x, int32 y)
{
	if (GetRangeValue(x, y) > 0 && GetRangeValue(x, y) <= SelectedAbility->Range)
	{
		/** Check for line of sight **/

		if (Owner->Grid->IsValidCoord(x, y))
		{
			/** Add Tiles **/
			AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

			/** Display Indicators **/
			FTransform transform;
			transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
			Owner->OnAbilityAim(transform);

			/** Add Characters **/
			FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y);
			if (AffectedTile.ActorOnTile != nullptr)
			{
				ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
				if (CharacterToAdd != nullptr)
				{
					AffectedCharacters.Add(CharacterToAdd);
					UE_LOG(LogTemp, Warning, TEXT("Added character"));
				}
			}
		}
	}
}

void UAbilityComponent::ResolveNonAttack(int32 x, int32 y)
{
	if (GetRangeValue(x, y) > 0 && GetRangeValue(x, y) <= SelectedAbility->Range)
	{
		AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

		FTransform transform;
		transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
		Owner->OnAbilityAim(transform);
	}
}

void UAbilityComponent::CancelAbility()
{
	SelectedAbility = nullptr;
	ResetSelectionData();
	Owner->CleanupAbilityIndicators();
	UE_LOG(LogTemp, Warning, TEXT("Ability Canceled"));
}

void UAbilityComponent::ResetSelectionData()
{
	// Tiles and characters
	AffectedCharacters.Empty();
	AffectedTiles.Empty();
	// Cone
	ConeDijkstraGrid.Empty();
	ConeDijkstraQueue.Empty();
	// Range
	RangeDijkstraGrid.Empty();
	RangeDijkstraQueue.Empty();
	//Sphere
	SphereDijkstraGrid.Empty();
	SphereDijkstraQueue.Empty();
}

void UAbilityComponent::ProcessNodeForSphere(int32 x, int32 y, float range)
{
	FDijkstraNode CurrentNode = SphereDijkstraGrid[Owner->Grid->CoordToIndex(x, y)];
	bool bQueueNeighbors = true;
	if (FMath::CeilToFloat(CurrentNode.NodeValue) == range)
	{
		bQueueNeighbors = false;
	}
	//Left
	if (y - 1 >= 0)
	{
		if (SphereDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].NodeValue > CurrentNode.NodeValue + 1.0f && SphereDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].bIsValidTerrain && !SphereDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].bWasProcessed)
		{
			SphereDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x, y - 1);
				SphereDijkstraQueue.Enqueue(pair);
			}
		}
	}
	//Right
	if (y + 1 <= Owner->Grid->Columns - 1)
	{
		if (SphereDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].NodeValue > CurrentNode.NodeValue + 1.0f && SphereDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].bIsValidTerrain && !SphereDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].bWasProcessed)
		{
			SphereDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x, y + 1);
				SphereDijkstraQueue.Enqueue(pair);
			}
		}
	}
	// Top
	if (x + 1 <= Owner->Grid->Rows - 1)
	{
		if (SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].NodeValue > CurrentNode.NodeValue + 1.0f && SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].bIsValidTerrain && !SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].bWasProcessed)
		{
			SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y);
				SphereDijkstraQueue.Enqueue(pair);
			}
		}
	}
	// Bottom
	if (x - 1 >= 0)
	{
		if (SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].NodeValue > CurrentNode.NodeValue + 1.0f && SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].bIsValidTerrain && !SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].bWasProcessed)
		{
			SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y);
				SphereDijkstraQueue.Enqueue(pair);
			}
		}
	}

	// Top-Left
	if (x + 1 <= Owner->Grid->Rows - 1 && y - 1 >= 0)
	{
		if (SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].NodeValue > CurrentNode.NodeValue + 1.5f && SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].bIsValidTerrain && !SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].bWasProcessed)
		{
			SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y - 1);
				SphereDijkstraQueue.Enqueue(pair);
			}
		}
	}

	// Top-Right
	if (x + 1 <= Owner->Grid->Rows - 1 && y + 1 <= Owner->Grid->Columns - 1)
	{
		if (SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].NodeValue > CurrentNode.NodeValue + 1.5f && SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].bIsValidTerrain && !SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].bWasProcessed)
		{
			SphereDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y + 1);
				SphereDijkstraQueue.Enqueue(pair);
			}
		}
	}
	// Bottom-Left
	if (x - 1 >= 0 && y - 1 >= 0)
	{
		if (SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].NodeValue > CurrentNode.NodeValue + 1.5f && SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].bIsValidTerrain && !SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].bWasProcessed)
		{
			SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y - 1);
				SphereDijkstraQueue.Enqueue(pair);
			}
		}
	}
	// Bottom-Right
	if (x - 1 >= 0 && y + 1 <= Owner->Grid->Columns - 1)
	{

		if (SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].NodeValue > CurrentNode.NodeValue + 1.5f && SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].bIsValidTerrain && !SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].bWasProcessed)
		{
			SphereDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y + 1);
				SphereDijkstraQueue.Enqueue(pair);
			}
		}
	}
	SphereDijkstraGrid[Owner->Grid->CoordToIndex(x, y)].bWasProcessed = true;

	if (Owner->Grid->IsValidCoord(x, y))
	{
		/** Add Tiles **/
		AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

		/** Display Indicators **/
		FTransform transform;
		transform.SetLocation(FVector((x* Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y* Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
		Owner->OnAbilityAim(transform);

		/** Add Characters **/
		FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y);
		if (AffectedTile.ActorOnTile != nullptr)
		{
			ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
			if (CharacterToAdd != nullptr)
			{
				AffectedCharacters.Add(CharacterToAdd);
				UE_LOG(LogTemp, Warning, TEXT("Added character"));
			}
		}
	}
}

TEnumAsByte<EConeDirection> UAbilityComponent::GetConeDirection(int32 x_char, int32 y_char, int32 x_curs, int32 y_curs)
{
	if (x_curs - x_char == y_curs - y_char) 
	{
		if (x_curs > x_char)
		{
			//UE_LOG(LogTemp, Warning, TEXT("UpRight"));
			return EConeDirection::D_UpRight;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("DownLeft"));
			return EConeDirection::D_DownLeft;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Undefined"));
		return EConeDirection::D_Undefined; // Redundant
	}
	else if (x_curs - x_char == (y_curs - y_char) * -1)
	{
		if (x_curs > x_char)
		{
			//UE_LOG(LogTemp, Warning, TEXT("UpLeft"));
			return EConeDirection::D_UpLeft;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("UpRight"));
			return EConeDirection::D_DownRight;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Undefined"));
		return EConeDirection::D_Undefined; // Redundant
	}
	else
	{
		// Up or displacement
		if (x_curs > x_char && y_curs > y_char - (x_curs - x_char) && y_curs < y_char + (x_curs - x_char))
		{
			if (x_curs - x_char > 2)
			{
				// Init Displacement
				int32 displacement;
				if (x_curs - x_char % 2 == 0)
				{
					// If even 
					displacement = (x_curs - x_char - 2) / 2;
				}
				else
				{
					// If odd
					displacement = (x_curs - x_char - 1) / 2;
				}

				if (y_curs <= y_char - (x_curs - x_char) + displacement)
				{
					//UE_LOG(LogTemp, Warning, TEXT("UpLeft"));
					return EConeDirection::D_UpLeft;
				}
				else if (y_curs >= y_char + (x_curs - x_char) - displacement)
				{
					//UE_LOG(LogTemp, Warning, TEXT("UpRight"));
					return EConeDirection::D_UpRight;
				}
			}

			//UE_LOG(LogTemp, Warning, TEXT("Up"));
			return EConeDirection::D_Up;
		}
		// Down or displacement
		else if (x_curs < x_char && y_curs > y_char - (x_char - x_curs) && y_curs < y_char + (x_char - x_curs))
		{
			if (x_char - x_curs > 2)
			{
				// Init Displacement
				int32 displacement;
				if (x_char - x_curs % 2 == 0)
				{
					// If even 
					displacement = (x_char - x_curs - 2) / 2;
				}
				else
				{
					// If odd
					displacement = (x_char - x_curs - 1) / 2;
				}

				if (y_curs <= y_char - (x_char - x_curs) + displacement)
				{
					//UE_LOG(LogTemp, Warning, TEXT("DownLeft"));
					return EConeDirection::D_DownLeft;
				}
				else if (y_curs >= y_char + (x_char - x_curs) - displacement)
				{
					//UE_LOG(LogTemp, Warning, TEXT("DownRight"));
					return EConeDirection::D_DownRight;
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT("Down"));
			return EConeDirection::D_Down;
		}
		// Right or displacement
		else if (y_curs > y_char && x_curs > x_curs - (y_curs - y_char) && x_curs < x_char + (y_curs - y_char))
		{
			if (y_curs - y_char > 2)
			{
				int32 displacement;
				if (y_curs - y_char % 2 == 0)
				{
					// If even 
					displacement = (y_curs - y_char - 2) / 2;
				}
				else
				{
					// If odd
					displacement = (y_curs - y_char - 1) / 2;
				}

				if (x_curs <= x_char - (y_curs - y_char) + displacement )
				{
					//UE_LOG(LogTemp, Warning, TEXT("DownRight"));
					return EConeDirection::D_DownRight;
				}
				else if (x_curs >= x_char + (y_curs - y_char) - displacement)
				{
					//UE_LOG(LogTemp, Warning, TEXT("UpRight"));
					return EConeDirection::D_UpRight;
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT("Right"));
			return EConeDirection::D_Right;
		}
		// Left or displacement
		else if (y_curs < y_char && x_curs > x_curs - (y_char - y_curs) && x_curs < x_curs + (y_char - y_curs))
		{
			if (y_char - y_curs > 2)
			{
				int32 displacement;
				if (y_char - y_curs % 2 == 0)
				{
					// If even 
					displacement = (y_char - y_curs - 2) / 2;
				}
				else
				{
					// If odd
					displacement = (y_char - y_curs - 1) / 2;
				}

				if (x_curs <= x_char - (y_char - y_curs) + displacement)
				{
					//UE_LOG(LogTemp, Warning, TEXT("DownLeft"));
					return EConeDirection::D_DownLeft;
				}
				else if (x_curs >= x_char + (y_char - y_curs) - displacement)
				{
					//UE_LOG(LogTemp, Warning, TEXT("UpLeft"));
					return EConeDirection::D_UpLeft;
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT("Left"));
			return EConeDirection::D_Left;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Undefined"));
		return EConeDirection::D_Undefined; // Redundant if everything is covered
	}
	//UE_LOG(LogTemp, Warning, TEXT("Undefined"));
	return EConeDirection::D_Undefined; // Redundant
}

void UAbilityComponent::ResolveConeHorizontal(int32 x_char, int32 y_char, TEnumAsByte<EConeDirection> Direction)
{
	int32 x = x_char;
	int32 y = y_char;
	if (Direction == EConeDirection::D_Right)
	{
		y = y + 1;
		for (int32 i = 1; i <= SelectedAbility->Range; i++)
		{
			FTransform transform;
			if (Owner->Grid->IsValidCoord(x, y))
			{
				/** Add Tiles **/
				AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

				/** Display Indicators **/
				transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
				Owner->OnAbilityAim(transform);

				/** Add Characters **/
				FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y);
				if (AffectedTile.ActorOnTile != nullptr)
				{
					ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
					if (CharacterToAdd != nullptr)
					{
						AffectedCharacters.Add(CharacterToAdd);
						UE_LOG(LogTemp, Warning, TEXT("Added character"));
					}
				}
			}

			int32 width;
			if (i % 2 == 0)
			{
				width = i / 2;
			}
			else
			{
				width = (i - 1) / 2;
			}

			for (int32 j = 1; j <= width; j++)
			{
				// To the Up
				if (Owner->Grid->IsValidCoord(x + j, y))
				{
					/** Add Tiles **/
					AffectedTiles.Add(Owner->Grid->CoordToIndex(x + j, y));

					/** Display Indicators **/
					transform.SetLocation(FVector(((x + j) * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
					Owner->OnAbilityAim(transform);

					/** Add Characters **/
					FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x + j, y);
					if (AffectedTile.ActorOnTile != nullptr)
					{
						ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
						if (CharacterToAdd != nullptr)
						{
							AffectedCharacters.Add(CharacterToAdd);
							UE_LOG(LogTemp, Warning, TEXT("Added character"));
						}
					}
				}

				// To the Down
				if (Owner->Grid->IsValidCoord(x - j, y))
				{
					/** Add Tiles **/
					AffectedTiles.Add(Owner->Grid->CoordToIndex(x - j, y));

					/** Display Indicators **/
					transform.SetLocation(FVector(((x - j) * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
					Owner->OnAbilityAim(transform);

					/** Add Characters **/
					FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x - j, y);
					if (AffectedTile.ActorOnTile != nullptr)
					{
						ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
						if (CharacterToAdd != nullptr)
						{
							AffectedCharacters.Add(CharacterToAdd);
							UE_LOG(LogTemp, Warning, TEXT("Added character"));
						}
					}
				}
			}
			y = y + 1;
		}
	}
	else if(Direction == EConeDirection::D_Left)
	{
		y = y - 1;
		for (int32 i = 1; i <= SelectedAbility->Range; i++)
		{
			FTransform transform;
			if (Owner->Grid->IsValidCoord(x, y))
			{
				/** Add Tiles **/
				AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

				/** Display Indicators **/
				transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
				Owner->OnAbilityAim(transform);

				/** Add Characters **/
				FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y);
				if (AffectedTile.ActorOnTile != nullptr)
				{
					ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
					if (CharacterToAdd != nullptr)
					{
						AffectedCharacters.Add(CharacterToAdd);
						UE_LOG(LogTemp, Warning, TEXT("Added character"));
					}
				}
			}

			int32 width;
			if (i % 2 == 0)
			{
				width = i / 2;
			}
			else
			{
				width = (i - 1) / 2;
			}

			for (int32 j = 1; j <= width; j++)
			{
				// To the Up
				if (Owner->Grid->IsValidCoord(x + j, y))
				{
					/** Add Tiles **/
					AffectedTiles.Add(Owner->Grid->CoordToIndex(x + j, y));

					/** Display Indicators **/
					transform.SetLocation(FVector(((x + j)* Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y* Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
					Owner->OnAbilityAim(transform);

					/** Add Characters **/
					FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x + j, y);
					if (AffectedTile.ActorOnTile != nullptr)
					{
						ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
						if (CharacterToAdd != nullptr)
						{
							AffectedCharacters.Add(CharacterToAdd);
							UE_LOG(LogTemp, Warning, TEXT("Added character"));
						}
					}
				}

				// To the Down
				if (Owner->Grid->IsValidCoord(x - j, y))
				{
					/** Add Tiles **/
					AffectedTiles.Add(Owner->Grid->CoordToIndex(x - j, y));

					/** Display Indicators **/
					transform.SetLocation(FVector(((x - j)* Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y* Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
					Owner->OnAbilityAim(transform);

					/** Add Characters **/
					FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x - j, y);
					if (AffectedTile.ActorOnTile != nullptr)
					{
						ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
						if (CharacterToAdd != nullptr)
						{
							AffectedCharacters.Add(CharacterToAdd);
							UE_LOG(LogTemp, Warning, TEXT("Added character"));
						}
					}
				}
			}
			y = y - 1;
		}
	}
}

void UAbilityComponent::ResolveConeVertical(int32 x_char, int32 y_char, TEnumAsByte<EConeDirection> Direction)
{
	int32 x = x_char;
	int32 y = y_char;
	if (Direction == EConeDirection::D_Up)
	{
		x = x + 1;
		for (int32 i = 1; i <= SelectedAbility->Range; i++)
		{
			FTransform transform;
			if (Owner->Grid->IsValidCoord(x, y))
			{
				/** Add Tiles **/
				AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

				/** Display Indicators **/
				transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
				Owner->OnAbilityAim(transform);

				/** Add Characters **/
				FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y);
				if (AffectedTile.ActorOnTile != nullptr)
				{
					ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
					if (CharacterToAdd != nullptr)
					{
						AffectedCharacters.Add(CharacterToAdd);
						UE_LOG(LogTemp, Warning, TEXT("Added character"));
					}
				}
			}

			int32 width;
			if (i % 2 == 0)
			{
				width = i / 2;
			}
			else
			{
				width = (i - 1) / 2;
			}

			for (int32 j = 1; j <= width; j++)
			{
				// To the right
				if (Owner->Grid->IsValidCoord(x, y + j))
				{
					/** Add Tiles **/
					AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y + j));

					/** Display Indicators **/
					transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), ((y + j) * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
					Owner->OnAbilityAim(transform);

					/** Add Characters **/
					FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y + j);
					if (AffectedTile.ActorOnTile != nullptr)
					{
						ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
						if (CharacterToAdd != nullptr)
						{
							AffectedCharacters.Add(CharacterToAdd);
							UE_LOG(LogTemp, Warning, TEXT("Added character"));
						}
					}
				}
				
				// To the left
				if (Owner->Grid->IsValidCoord(x, y - j))
				{
					/** Add Tiles **/
					AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y - j));

					/** Display Indicators **/
					transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), ((y - j) * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
					Owner->OnAbilityAim(transform);

					/** Add Characters **/
					FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y - j);
					if (AffectedTile.ActorOnTile != nullptr)
					{
						ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
						if (CharacterToAdd != nullptr)
						{
							AffectedCharacters.Add(CharacterToAdd);
							UE_LOG(LogTemp, Warning, TEXT("Added character"));
						}
					}
				}
			}
			x = x + 1;
		}
	}
	else if (Direction == EConeDirection::D_Down)
	{
		x = x - 1;
		for (int32 i = 1; i <= SelectedAbility->Range; i++)
		{
			FTransform transform;
			if (Owner->Grid->IsValidCoord(x, y))
			{
				/** Add Tiles **/
				AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

				/** Display Indicators **/
				transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
				Owner->OnAbilityAim(transform);

				/** Add Characters **/
				FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y);
				if (AffectedTile.ActorOnTile != nullptr)
				{
					ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
					if (CharacterToAdd != nullptr)
					{
						AffectedCharacters.Add(CharacterToAdd);
						UE_LOG(LogTemp, Warning, TEXT("Added character"));
					}
				}
			}

			int32 width;
			if (i % 2 == 0)
			{
				width = i / 2;
			}
			else
			{
				width = (i - 1) / 2;
			}

			for (int32 j = 1; j <= width; j++)
			{
				// To the right
				if (Owner->Grid->IsValidCoord(x, y + j))
				{
					/** Add Tiles **/
					AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y + j));

					/** Display Indicators **/
					transform.SetLocation(FVector((x* Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), ((y + j)* Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
					Owner->OnAbilityAim(transform);

					/** Add Characters **/
					FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y + j);
					if (AffectedTile.ActorOnTile != nullptr)
					{
						ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
						if (CharacterToAdd != nullptr)
						{
							AffectedCharacters.Add(CharacterToAdd);
							UE_LOG(LogTemp, Warning, TEXT("Added character"));
						}
					}
				}

				// To the left
				if (Owner->Grid->IsValidCoord(x, y - j))
				{
					/** Add Tiles **/
					AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y - j));

					/** Display Indicators **/
					transform.SetLocation(FVector((x* Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), ((y - j)* Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
					Owner->OnAbilityAim(transform);

					/** Add Characters **/
					FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y - j);
					if (AffectedTile.ActorOnTile != nullptr)
					{
						ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
						if (CharacterToAdd != nullptr)
						{
							AffectedCharacters.Add(CharacterToAdd);
							UE_LOG(LogTemp, Warning, TEXT("Added character"));
						}
					}
				}
			}
			x = x - 1;
		}
	}
}

void UAbilityComponent::ResolveConeDiagonal(int32 x_char, int32 y_char, TEnumAsByte<EConeDirection> Direction)
{
	/** Init the Dijkstra grid for the cone algorithm **/
	ConeDijkstraGrid.Empty();

	for (int32 i = 0; i < Owner->Grid->Rows; i++)
	{
		for (int32 j = 0; j < Owner->Grid->Columns; j++)
		{
			FDijkstraNode node;
			node.x = i;
			node.y = j;
			node.bWasProcessed = false;
			node.bIsValidTerrain = true;

			if (x_char == i && y_char == j)
			{
				node.NodeValue = 0;
			}
			else
			{
				node.NodeValue = Owner->Grid->Rows * Owner->Grid->Columns + 1;
			}
			ConeDijkstraGrid.Add(node);
		}
	}

	TPair<int32, int32> s = TPair<int32, int32>(x_char, y_char);
	ConeDijkstraQueue.Enqueue(s);

	while (!ConeDijkstraQueue.IsEmpty())
	{
		TPair<int32, int32> pair;
		ConeDijkstraQueue.Dequeue(pair);
		ProcessNodeForCone(pair.Key, pair.Value, SelectedAbility->Range - 1, Direction);
	}
}

void UAbilityComponent::ProcessNodeForCone(int32 x, int32 y, float range, TEnumAsByte<EConeDirection> Direction)
{
	FDijkstraNode CurrentNode = ConeDijkstraGrid[Owner->Grid->CoordToIndex(x, y)];
	bool bQueueNeighbors = true;
	if (FMath::CeilToFloat(CurrentNode.NodeValue) == range)
	{
		bQueueNeighbors = false;
	}
	//Left
	if (y - 1 >= 0 && (Direction == EConeDirection::D_DownLeft || Direction == EConeDirection::D_UpLeft ))
	{
		if (ConeDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].NodeValue > CurrentNode.NodeValue + 1.0f && ConeDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].bIsValidTerrain && !ConeDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].bWasProcessed)
		{
			ConeDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x, y - 1);
				ConeDijkstraQueue.Enqueue(pair);
				//UE_LOG(LogTemp, Warning, TEXT("Left Queued"));
			}
		}
	}
	//Right
	if (y + 1 <= Owner->Grid->Columns - 1 && (Direction == EConeDirection::D_UpRight || Direction == EConeDirection::D_DownRight))
	{
		if (ConeDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].NodeValue > CurrentNode.NodeValue + 1.0f && ConeDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].bIsValidTerrain && !ConeDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].bWasProcessed)
		{
			ConeDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x, y + 1);
				ConeDijkstraQueue.Enqueue(pair);
				//UE_LOG(LogTemp, Warning, TEXT("Right Queued"));
			}
		}
	}
	// Top
	if (x + 1 <= Owner->Grid->Rows - 1 && (Direction == EConeDirection::D_UpLeft || Direction == EConeDirection::D_UpRight))
	{
		if (ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].NodeValue > CurrentNode.NodeValue + 1.0f && ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].bIsValidTerrain && !ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].bWasProcessed)
		{
			ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y);
				ConeDijkstraQueue.Enqueue(pair);
				//UE_LOG(LogTemp, Warning, TEXT("Top Queued"));
			}
		}
	}
	// Bottom
	if (x - 1 >= 0 && (Direction == EConeDirection::D_DownLeft || Direction == EConeDirection::D_DownRight))
	{
		if (ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].NodeValue > CurrentNode.NodeValue + 1.0f && ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].bIsValidTerrain && !ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].bWasProcessed)
		{
			ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y);
				ConeDijkstraQueue.Enqueue(pair);
				//UE_LOG(LogTemp, Warning, TEXT("Bottom Queued"));
			}
		}
	}
	// Top-Left
	if (x + 1 <= Owner->Grid->Rows - 1 && y - 1 >= 0 && Direction == EConeDirection::D_UpLeft)
	{
		if (ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].NodeValue > CurrentNode.NodeValue + 1.5f && ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].bIsValidTerrain && !ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].bWasProcessed)
		{
			ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y - 1);
				ConeDijkstraQueue.Enqueue(pair);
				//UE_LOG(LogTemp, Warning, TEXT("Top Left Queued"));
			}
		}
	}
	// Top-Right
	if (x + 1 <= Owner->Grid->Rows - 1 && y + 1 <= Owner->Grid->Columns - 1 && Direction == EConeDirection::D_UpRight)
	{
		if (ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].NodeValue > CurrentNode.NodeValue + 1.5f && ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].bIsValidTerrain && !ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].bWasProcessed)
		{
			ConeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y + 1);
				ConeDijkstraQueue.Enqueue(pair);
				//UE_LOG(LogTemp, Warning, TEXT("Top Right Queued"));
			}
		}
	}
	// Bottom-Left
	if (x - 1 >= 0 && y - 1 >= 0 && Direction == EConeDirection::D_DownLeft)
	{
		if (ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].NodeValue > CurrentNode.NodeValue + 1.5f && ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].bIsValidTerrain && !ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].bWasProcessed)
		{
			ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y - 1);
				ConeDijkstraQueue.Enqueue(pair);
				//UE_LOG(LogTemp, Warning, TEXT("Bottom Left Queued"));
			}
		}
	}
	// Bottom-Right
	if (x - 1 >= 0 && y + 1 <= Owner->Grid->Columns - 1 && Direction == EConeDirection::D_DownRight)
	{
		if (ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].NodeValue > CurrentNode.NodeValue + 1.5f && ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].bIsValidTerrain && !ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].bWasProcessed)
		{
			ConeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y + 1);
				ConeDijkstraQueue.Enqueue(pair);
			}
		}
	}

	ConeDijkstraGrid[Owner->Grid->CoordToIndex(x, y)].bWasProcessed = true;
	
	if (Owner->Grid->IsValidCoord(x, y))
	{
		/** Add Tiles **/
		AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

		/** Display Indicators **/
		FTransform transform;
		transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
		Owner->OnAbilityAim(transform);

		/** Add Characters **/
		FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y);
		if (AffectedTile.ActorOnTile != nullptr)
		{
			ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
			if (CharacterToAdd != nullptr)
			{
				AffectedCharacters.Add(CharacterToAdd);
				UE_LOG(LogTemp, Warning, TEXT("Added character"));
			}
		}
	}
}

void UAbilityComponent::PlotTileLow(int32 x0, int32 y0, int32 x1, int32 y1, bool bWasSwitched)
{
	int32 dx = x1 - x0;
	int32 dy = y1 - y0;
	int32 yi = 1;
	if (dy < 0)
	{
		yi = -1;
		dy = dy*-1;
	}
	int32 D = (2 * dy) - dx;
	int32 y = y0;

	for (int32 x = x0; x <= x1; x++)
	{
		// If we switched then we exclude the last tile
		if ((!bWasSwitched && x == x0 && y == y0) || (bWasSwitched && x == x1 && y == y1))
		{
			// Skip
			//UE_LOG(LogTemp, Warning, TEXT("P_LOW Skipped (%i,%i)"), x, y);
		}
		else
		{
			if (Owner->Grid->IsValidCoord(x, y) && GetRangeValue(x, y) <= SelectedAbility->Range)
			{
				/** Add Tiles **/
				AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

				/** Display Indicators **/
				FTransform transform;
				transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
				Owner->OnAbilityAim(transform);

				/** Add Characters **/
				FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y);
				if (AffectedTile.ActorOnTile != nullptr)
				{
					ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
					if (CharacterToAdd != nullptr)
					{
						AffectedCharacters.Add(CharacterToAdd);
						UE_LOG(LogTemp, Warning, TEXT("Added character"));
					}
				}
			}
		}

		if (D > 0)
		{
			y = y + yi;
			D = D + (2 * (dy - dx));
		}
		else
		{
			D = D + 2 * dy;
		}
	}
}

void UAbilityComponent::PlotTileHigh(int32 x0, int32 y0, int32 x1, int32 y1, bool bWasSwitched)
{
	int32 dx = x1 - x0;
	int32 dy = y1 - y0;
	int32 xi = 1;
	if (dx < 0)
	{
		xi = -1;
		dx = dx * -1;
	}
	int32 D = (2 * dx) - dy;
	int32 x = x0;

	for (int32 y = y0; y <= y1; y++)
	{
		if ((!bWasSwitched && x == x0 && y == y0) || (bWasSwitched && x == x1 && y == y1))
		{
			// Skip
			//UE_LOG(LogTemp, Warning, TEXT("P_HIGH Skipped (%i,%i)"), x, y);
		}
		else
		{
			if (Owner->Grid->IsValidCoord(x, y) && GetRangeValue(x, y) <= SelectedAbility->Range)
			{
				/** Add Tiles **/
				AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));

				/** Display Indicators **/
				FTransform transform;
				transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 0.1f));
				Owner->OnAbilityAim(transform);

				/** Add Characters **/
				FTileProperties AffectedTile = Owner->Grid->GetTilePropertiesFromCoord(x, y);
				if (AffectedTile.ActorOnTile != nullptr)
				{
					ABaseCharacter* CharacterToAdd = Cast<ABaseCharacter>(AffectedTile.ActorOnTile);
					if (CharacterToAdd != nullptr)
					{
						AffectedCharacters.Add(CharacterToAdd);
						UE_LOG(LogTemp, Warning, TEXT("Added character"));
					}
				}
			}
		}

		if (D > 0)
		{
			x = x + xi;
			D = D + (2 * (dx - dy));
		}
		else
		{
			D = D + 2 * dx;
		}
	}
}

void UAbilityComponent::InitRangeGrid(int32 x_char, int32 y_char)
{
	if (RangeDijkstraGrid.Num() > 0)
	{
		RangeDijkstraGrid.Empty();
	}

	for (int32 i = 0; i < Owner->Grid->Rows; i++)
	{
		for (int32 j = 0; j < Owner->Grid->Columns; j++)
		{
			FDijkstraNode node;
			node.x = i;
			node.y = j;
			node.bWasProcessed = false;
			node.bIsValidTerrain = true;

			if (x_char == i && y_char == j)
			{
				node.NodeValue = 0;
			}
			else
			{
				node.NodeValue = Owner->Grid->Rows * Owner->Grid->Columns + 1;
			}
			RangeDijkstraGrid.Add(node);
		}
	}

	TPair<int32, int32> s = TPair<int32, int32>(x_char, y_char);
	RangeDijkstraQueue.Enqueue(s);
	while (RangeDijkstraQueue.IsEmpty() == false)
	{
		TPair<int32, int32> pair;
		RangeDijkstraQueue.Dequeue(pair);
		ProcessNodeForRange(pair.Key, pair.Value, SelectedAbility->Range);
	}
}

void UAbilityComponent::ProcessNodeForRange(int32 x, int32 y, float range)
{
	FDijkstraNode CurrentNode = RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y)];
	bool bQueueNeighbors = true;
	if (FMath::CeilToFloat(CurrentNode.NodeValue) == range)
	{
		bQueueNeighbors = false;
	}
	//Left
	if (y - 1 >= 0)
	{
		if (RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].NodeValue > CurrentNode.NodeValue + 1.0f && RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].bIsValidTerrain && !RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].bWasProcessed)
		{
			RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y - 1)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x, y - 1);
				RangeDijkstraQueue.Enqueue(pair);
			}
		}
	}
	//Right
	if (y + 1 <= Owner->Grid->Columns - 1)
	{
		if (RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].NodeValue > CurrentNode.NodeValue + 1.0f && RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].bIsValidTerrain && !RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].bWasProcessed)
		{
			RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y + 1)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x, y + 1);
				RangeDijkstraQueue.Enqueue(pair);
			}
		}
	}
	// Top
	if (x + 1 <= Owner->Grid->Rows - 1)
	{
		if (RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].NodeValue > CurrentNode.NodeValue + 1.0f && RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].bIsValidTerrain && !RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].bWasProcessed)
		{
			RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y);
				RangeDijkstraQueue.Enqueue(pair);
			}
		}
	}
	// Bottom
	if (x - 1 >= 0)
	{
		if (RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].NodeValue > CurrentNode.NodeValue + 1.0f && RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].bIsValidTerrain && !RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].bWasProcessed)
		{
			RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y)].NodeValue = CurrentNode.NodeValue + 1.0f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.0f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y);
				RangeDijkstraQueue.Enqueue(pair);
			}
		}
	}

	// Top-Left
	if (x + 1 <= Owner->Grid->Rows - 1 && y - 1 >= 0)
	{
		if (RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].NodeValue > CurrentNode.NodeValue + 1.5f && RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].bIsValidTerrain && !RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].bWasProcessed)
		{
			RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y - 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y - 1);
				RangeDijkstraQueue.Enqueue(pair);
			}
		}
	}

	// Top-Right
	if (x + 1 <= Owner->Grid->Rows - 1 && y + 1 <= Owner->Grid->Columns - 1)
	{
		if (RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].NodeValue > CurrentNode.NodeValue + 1.5f && RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].bIsValidTerrain && !RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].bWasProcessed)
		{
			RangeDijkstraGrid[Owner->Grid->CoordToIndex(x + 1, y + 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x + 1, y + 1);
				RangeDijkstraQueue.Enqueue(pair);
			}
		}
	}

	// Bottom-Left
	if (x - 1 >= 0 && y - 1 >= 0)
	{
		if (RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].NodeValue > CurrentNode.NodeValue + 1.5f && RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].bIsValidTerrain && !RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].bWasProcessed)
		{
			RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y - 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y - 1);
				RangeDijkstraQueue.Enqueue(pair);
			}
		}
	}
	// Bottom-Right
	if (x - 1 >= 0 && y + 1 <= Owner->Grid->Columns - 1)
	{

		if (RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].NodeValue > CurrentNode.NodeValue + 1.5f && RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].bIsValidTerrain && !RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].bWasProcessed)
		{
			RangeDijkstraGrid[Owner->Grid->CoordToIndex(x - 1, y + 1)].NodeValue = CurrentNode.NodeValue + 1.5f;
			if (bQueueNeighbors && CurrentNode.NodeValue + 1.5f <= range)
			{
				TPair<int32, int32> pair = TPair<int32, int32>(x - 1, y + 1);
				RangeDijkstraQueue.Enqueue(pair);
			}
		}
	}
	RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y)].bWasProcessed = true;
}

float UAbilityComponent::GetRangeValue(int32 x, int32 y)
{
	if (Owner != nullptr && Owner->Grid != nullptr)
	{
		return RangeDijkstraGrid[Owner->Grid->CoordToIndex(x, y)].NodeValue;
	}
	return -1.0f;
}