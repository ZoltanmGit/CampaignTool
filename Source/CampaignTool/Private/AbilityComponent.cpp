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
	if (HasSelectedAbility() && Owner != nullptr && Owner->Grid != nullptr)
	{
		Owner->CleanupAbilityIndicators();
		if (SelectedAbility->TargetType == ETargetType::AOE)
		{
			UBaseAoeTargetAbility* AbilityAsAOE = Cast<UBaseAoeTargetAbility>(SelectedAbility);
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
				
				// Line
				ResolveLine(x0, y0, x1, y1);
			}
			else if (AbilityAsAOE != nullptr && AbilityAsAOE->AreaEffectType == EAreaOfEffectType::Sphere)
			{
				// Sphere
			}
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
			UBaseSingleTargetAbility* AbilityAsSingle = Cast<UBaseSingleTargetAbility>(SelectedAbility);
			if (AbilityAsSingle != nullptr && AbilityAsSingle->SingleTargetType == ESingleTargetType::NonAttack)
			{
				// Non Attack
			}
			if (AbilityAsSingle != nullptr && AbilityAsSingle->SingleTargetType == ESingleTargetType::MeleeAttack)
			{
				// Melee Attack
			}
			if (AbilityAsSingle != nullptr && AbilityAsSingle->SingleTargetType == ESingleTargetType::RangedAttack)
			{
				// Ranged Attack
			}
		}
	}
}

void UAbilityComponent::ExecuteAbility()
{
	if (SelectedAbility != nullptr && AffectedCharacters.Num() > 0)
	{
		if (SelectedAbility->TargetType == ETargetType::AOE)
		{
			UBaseAoeTargetAbility* AOEAbility = Cast<UBaseAoeTargetAbility>(SelectedAbility);
			AOEAbility->AffectedCharacters = AffectedCharacters;
			AOEAbility->Execute();
		}
		else if (SelectedAbility->TargetType == ETargetType::Single && AffectedCharacters[0] != nullptr)
		{
			UBaseSingleTargetAbility* SingleTargetAbility = Cast<UBaseSingleTargetAbility>(SelectedAbility);
			SingleTargetAbility->Target = AffectedCharacters[0];
			SingleTargetAbility->Execute();
		}
		CancelAbility();
		Owner->bIsAimingAbility = false;
	}
}

void UAbilityComponent::SelectAbility(UBaseAbility* AbilityToSelect)
{
	SelectedAbility = AbilityToSelect;
}

void UAbilityComponent::ResolveLine(int32 x0, int32 y0, int32 x1, int32 y1)
{
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
}

void UAbilityComponent::CancelAbility()
{
	SelectedAbility = nullptr;
	ResetPathfinding();
	Owner->CleanupAbilityIndicators();
	UE_LOG(LogTemp, Warning, TEXT("Ability Canceled"));
}

void UAbilityComponent::ResetPathfinding()
{
	if (DijkstraGrid.Num() > 0)
	{
		DijkstraGrid.Empty();
	}
	if (!DijkstraQueue.IsEmpty())
	{
		DijkstraQueue.Empty();
	}
	if (AffectedCharacters.Num() > 0)
	{
		AffectedCharacters.Empty();
	}
	if (AffectedTiles.Num() > 0)
	{
		AffectedTiles.Empty();
	}
}

bool UAbilityComponent::HasSelectedAbility()
{
	if (SelectedAbility != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

TEnumAsByte<EConeDirection> UAbilityComponent::GetConeDirection(int32 x_char, int32 y_char, int32 x_curs, int32 y_curs)
{
	if (x_curs - x_char == y_curs - y_char) 
	{
		if (x_curs > x_char)
		{
			UE_LOG(LogTemp, Warning, TEXT("UpRight"));
			return EConeDirection::D_UpRight;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DownLeft"));
			return EConeDirection::D_DownLeft;
		}
		UE_LOG(LogTemp, Warning, TEXT("Undefined"));
		return EConeDirection::D_Undefined; // Redundant
	}
	else if (x_curs - x_char == (y_curs - y_char) * -1)
	{
		if (x_curs > x_char)
		{
			UE_LOG(LogTemp, Warning, TEXT("UpLeft"));
			return EConeDirection::D_UpLeft;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UpRight"));
			return EConeDirection::D_DownRight;
		}
		UE_LOG(LogTemp, Warning, TEXT("Undefined"));
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
					UE_LOG(LogTemp, Warning, TEXT("UpLeft"));
					return EConeDirection::D_UpLeft;
				}
				else if (y_curs >= y_char + (x_curs - x_char) - displacement)
				{
					UE_LOG(LogTemp, Warning, TEXT("UpRight"));
					return EConeDirection::D_UpRight;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Up"));
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
					UE_LOG(LogTemp, Warning, TEXT("DownLeft"));
					return EConeDirection::D_DownLeft;
				}
				else if (y_curs >= y_char + (x_char - x_curs) - displacement)
				{
					UE_LOG(LogTemp, Warning, TEXT("DownRight"));
					return EConeDirection::D_DownRight;
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("Down"));
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
					UE_LOG(LogTemp, Warning, TEXT("DownRight"));
					return EConeDirection::D_DownRight;
				}
				else if (x_curs >= x_char + (y_curs - y_char) - displacement)
				{
					UE_LOG(LogTemp, Warning, TEXT("UpRight"));
					return EConeDirection::D_UpRight;
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("Right"));
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
					UE_LOG(LogTemp, Warning, TEXT("DownLeft"));
					return EConeDirection::D_DownLeft;
				}
				else if (x_curs >= x_char + (y_char - y_curs) - displacement)
				{
					UE_LOG(LogTemp, Warning, TEXT("UpLeft"));
					return EConeDirection::D_UpLeft;
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("Left"));
			return EConeDirection::D_Left;
		}
		UE_LOG(LogTemp, Warning, TEXT("Undefined"));
		return EConeDirection::D_Undefined; // Redundant if everything is covered
	}
	UE_LOG(LogTemp, Warning, TEXT("Undefined"));
	return EConeDirection::D_Undefined; // Redundant
}

void UAbilityComponent::ResolveConeHorizontal(int32 x_char, int32 y_char, TEnumAsByte<EConeDirection> Direction)
{
}

void UAbilityComponent::ResolveConeVertical(int32 x_char, int32 y_char, TEnumAsByte<EConeDirection> Direction)
{
	int32 x = x_char;
	int32 y = y_char;
	if (Direction == EConeDirection::D_Up)
	{
		x = x + 1;
		for (int32 i = 1; i <= 6/* CHANGE TO ABILITY RANGE*/; i++)
		{
			FTransform transform;
			if (Owner->Grid->IsValidCoord(x, y))
			{
				transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 10.0f));
				Owner->OnAbilityAim(transform);
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
					transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), ((y + j) * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 10.0f));
					Owner->OnAbilityAim(transform);
				}
				
				// To the left
				if (Owner->Grid->IsValidCoord(x, y - 1))
				{
					transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), ((y - j) * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 10.0f));
					Owner->OnAbilityAim(transform);
				}
			}
			x = x + 1;
		}
	}
	else if (Direction == EConeDirection::D_Down)
	{
		x = x - 1;
		for (int32 i = 1; i <= 9/* CHANGE TO ABILITY RANGE*/; i++)
		{
			FTransform transform;
			if (Owner->Grid->IsValidCoord(x, y))
			{
				transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 10.0f));
				Owner->OnAbilityAim(transform);
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
					transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), ((y + j) * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 10.0f));
					Owner->OnAbilityAim(transform);
				}

				// To the left
				if (Owner->Grid->IsValidCoord(x, y - 1))
				{
					transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), ((y - j) * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 10.0f));
					Owner->OnAbilityAim(transform);
				}
			}
			x = x - 1;
		}
	}
}

void UAbilityComponent::ResolveConeDiagonal(int32 x_char, int32 y_char, TEnumAsByte<EConeDirection> Direction)
{
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
			UE_LOG(LogTemp, Warning, TEXT("P_LOW Skipped (%i,%i)"), x, y);
		}
		else
		{
			AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));
			UE_LOG(LogTemp, Warning, TEXT("P_LOW (%i,%i)"), x, y);
			FTransform transform;
			transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 10.0f));
			Owner->OnAbilityAim(transform);

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
			UE_LOG(LogTemp, Warning, TEXT("P_HIGH Skipped (%i,%i)"), x, y);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("P_HIGH (%i,%i)"), x, y);
			AffectedTiles.Add(Owner->Grid->CoordToIndex(x, y));
			//Display selection
			FTransform transform;
			transform.SetLocation(FVector((x * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), (y * Owner->Grid->fieldSize) + (Owner->Grid->fieldSize / 2), 10.0f));
			Owner->OnAbilityAim(transform);

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

void UAbilityComponent::ProcessTile()
{
}

