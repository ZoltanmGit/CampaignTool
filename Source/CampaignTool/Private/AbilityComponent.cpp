// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "BaseAbility.h"
#include "BaseCharacter.h"
#include "BaseAoeTargetAbility.h"
#include "BaseCharacter.h"
#include "IndicatorActor.h"
#include "BaseSingleTargetAbility.h"


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

void UAbilityComponent::HandleTileChange(int32 x, int32 y, float range)
{
}

void UAbilityComponent::ExecuteAbility()
{
	switch (SelectedAbility->TargetType)
	{
	case ETargetType::AOE:
		if (AffectedCharacters.Num() > 0)
		{
			UBaseAoeTargetAbility* AOEAbility = Cast<UBaseAoeTargetAbility>(SelectedAbility);
			AOEAbility->AffectedCharacters = AffectedCharacters;
			AOEAbility->Execute();
		}
		break;
	case ETargetType::Single:
		
		if (AffectedCharacters.IsValidIndex(0))
		{
			if (AffectedCharacters[0] != nullptr)
			{
				UBaseSingleTargetAbility* SingleTargetAbility = Cast<UBaseSingleTargetAbility>(SelectedAbility);
				SingleTargetAbility->Target = AffectedCharacters[0];
				SingleTargetAbility->Execute();
			}
		}
		break;
	default:

		break;
	}
}

void UAbilityComponent::OnAbilitySelection(UBaseAbility* AbilityToSelect)
{

}

void UAbilityComponent::OnAbilityDeselection()
{
}

void UAbilityComponent::GetAffectedCharactersInLine(int32 x0, int32 y0, int32 x1, int32 y1)
{
	Owner->CleanupAbilityIndicators();
	if (abs(y1 - y0) < abs(x1 - x0))
	{

		if (x0 > x1)
		{
			PlotTileLow(x1, y1, x0, y0);
		}
		else
		{
			PlotTileLow(x0, y0, x1, y1);
		}
	}
	else
	{
		if (y0 > y1)
		{
			PlotTileHigh(x1, y1, x0, y0);
		}
		else
		{
			PlotTileHigh(x0, y0, x1, y1);
		}
	}
}

void UAbilityComponent::PlotTileLow(int32 x0, int32 y0, int32 x1, int32 y1)
{
	//int32 range = SelectedAbility->Range;
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
	for (int32 x = x0; x < x1; x++)
	{
		//Or stop if wall or range
		FTransform transform;
		transform.SetLocation(FVector((x * 100) + (100 / 2), (y * 100) + (100 / 2), 0.0f));
		Owner->OnPathfinding(transform);
		UE_LOG(LogTemp, Warning, TEXT("P_LOW (%i ; %i)"), x, y);
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

void UAbilityComponent::PlotTileHigh(int32 x0, int32 y0, int32 x1, int32 y1)
{
	//int32 range = SelectedAbility->Range;
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
	for (int32 y = y0; y < y1; y++)
	{
		//Or stop if wall or range
		UE_LOG(LogTemp, Warning, TEXT("P_HIGH (%i ; %i)"), x, y);
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

