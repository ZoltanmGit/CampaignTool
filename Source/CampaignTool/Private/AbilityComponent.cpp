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
	int32 range = SelectedAbility->Range;
	int32 dx = x1-x0;
	int32 dy = y1-y0;
	int32 D = 2*dy - dx;
	int32 y = y0;
	for(int i = x0; i <= x1 /*|| range>0*/; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Plot (%i,%i)",i,));
		//range--;
		if (D > 0)
		{
			y++;
			D = D - 2 * dx;
		}
		D = D + 2 * dy;
	}
}

void UAbilityComponent::ProcessTile()
{
}

