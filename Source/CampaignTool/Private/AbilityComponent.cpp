// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "BaseAbility.h"
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

void UAbilityComponent::GetAffectedCharactersInLine()
{
}

void UAbilityComponent::ProcessTile()
{
}

