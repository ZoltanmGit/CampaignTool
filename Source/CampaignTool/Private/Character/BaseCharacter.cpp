// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Components/SplineComponent.h"
#include "Character/HealthComponent.h"
#include "AbilitySystem/AbilityComponent.h"
#include "AbilitySystem/AbilityStorage.h"
#include "Character/AttributesComponent.h"
#include "MovementSystem/PathfinderComponent.h"
#include "MovementSystem/MoverComponent.h"
#include "InventorySystem/CharacterInventoryComponent.h"
#include "InventorySystem/ItemStorage.h"
#include "GridSystem/Grid.h"
#include "Utilities/IndicatorActor.h"
#include "Utilities/DiceRoller.h"
#include "Kismet/GameplayStatics.h"
#include "../../CampaignToolGameModeBase.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** Character Mesh **/
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	CharacterMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CharacterMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	CharacterMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // So character's meshes can overlap without the collision going bonkers
	RootComponent = CharacterMesh; //Set Mesh as the RootComponent
	
	/** Movement **/
	Mover = CreateDefaultSubobject<UMoverComponent>(TEXT("MoverComponent"));
	Pathfinder = CreateDefaultSubobject<UPathfinderComponent>(TEXT("PathFinder"));
	
	
	/** General Components **/
	CharacterHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("CharacterHealth"));
	CharacterAttributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("CharacterAttributes"));
	CharacterInventory = CreateDefaultSubobject<UCharacterInventoryComponent>(TEXT("CharacterInventory"));
	DiceRoller = CreateDefaultSubobject<UDiceRoller>(TEXT("DiceRoller"));
	CharacterAbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
	Grid = nullptr; // TODO: Check if redundant

	Initiative = 0;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CharacterHealth != nullptr)
	{
		CharacterHealth->SetFullHealth(CharacterAttributes->Stats.HitDie);
		CharacterHealth->SetCurrentHealth(CharacterHealth->GetFullHealth());
		OnHealthChange();
	}
	if (CharacterInventory != nullptr)
	{
		OnStatChange();
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::BeginTurn()
{
	if (Grid && CharacterHealth && CharacterAttributes && Pathfinder && Mover && !bCanAct && !bCanMove && !bIsActive && Indicator)
	{
		UE_LOG(LogTemp, Warning, TEXT("Components are valid at BeginTurn"));
		bCanAct = true;
		bCanMove = true;
		bIsActive = true;
		CurrentSpeed = CharacterAttributes->Stats.Speed / 5.0f;
		//RefreshPathfinding();

		OnStatChange();
	}
}

void ABaseCharacter::EndTurn()
{
	bCanAct = false;
	bCanMove = false;
	bIsActive = false;
	CleanupPathfinding();
	UE_LOG(LogTemp, Warning, TEXT("Turn Ended"));

	ACampaignToolGameModeBase* GameMode = Cast<ACampaignToolGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->NextTurn();
}

void ABaseCharacter::ChangeLocation(FVector newLocation)
{
	if (Grid && Pathfinder && Mover && bCanMove && bCanAct)
	{
		bCanAct = false;
		bCanMove = false;
		FTransform TempTransform;
		TempTransform.SetLocation(newLocation);
		int32 index;
		Grid->GetTilePropertiesFromTransform(TempTransform, index);
		if (Pathfinder->ValidIndexMap.Contains(index) && Mover->Timeline)
		{
			float minusSpeed = *Pathfinder->ValidIndexMap.Find(index);
			CurrentSpeed = CurrentSpeed - minusSpeed;

			int32 CurrentIndex;
			FTileProperties CurrentTileData = Grid->GetTilePropertiesFromTransform(this->GetActorTransform(), CurrentIndex);
			Grid->GridDataArray[CurrentIndex].bIsOccupied = false;
			Grid->GridDataArray[CurrentIndex].ActorOnTile = nullptr;

			int32 NewTileIndex;
			FTileProperties NewLocationTileData = Grid->GetTilePropertiesFromTransform(TempTransform, NewTileIndex);
			Grid->GridDataArray[NewTileIndex].bIsOccupied = true;
			Grid->GridDataArray[NewTileIndex].ActorOnTile = this;
			
			
			Mover->Timeline->SetPlayRate((1/minusSpeed)*4);
			Mover->MoveCharacter(newLocation);
			CharacterLocation = newLocation;

			OnStatChange(); // To update UI about speed
		}
		else
		{
			bCanAct = true;
			bCanMove = true;
			UE_LOG(LogTemp, Warning, TEXT("Invalid movement change."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot move"));
	}
}

void ABaseCharacter::RefreshPathfinding()
{
	if (Pathfinder && Mover)
	{
		int32 index;//Unused
		FTileProperties tile = Grid->GetTilePropertiesFromTransform(this->GetActorTransform(), index);
		Pathfinder->GetValidMovementIndexes(tile.Row, tile.Column, CurrentSpeed);


		for (TPair<int32, float>& Kvp : Pathfinder->ValidIndexMap) //Source on iteration: https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/#range-basedfor
		{
			float i;
			float j;
			i = Kvp.Key / Grid->Columns;
			j = Kvp.Key % Grid->Columns;
			FTransform Transform;
			Transform.SetLocation(FVector((i * Grid->fieldSize) + (Grid->fieldSize / 2), (j * Grid->fieldSize) + (Grid->fieldSize / 2), 5.0f));
			OnPathfinding(Transform);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pathfinder is nullptr"));
	}
}

void ABaseCharacter::InitializeCharacter(FCharacterStruct Character, AGrid* ArgGrid, AIndicatorActor* ArgIndicator, AAbilityStorage* ArgAbilityStorage, AItemStorage* ArgItemStorage)
{
	/** Initializing utility actors **/
	Grid = ArgGrid;
	Indicator = ArgIndicator;
	
	/** Passing Grid information to pathfinder **/
	Pathfinder->Rows = Grid->Rows;
	Pathfinder->Columns = Grid->Columns;
	
	/** Initializing Health and Attributes **/
	if (CharacterAttributes != nullptr && CharacterHealth != nullptr)
	{
		CharacterHealth->Owner = this;
		CharacterAttributes->InitComponent(Character);
		if (CharacterHealth)
		{
			CharacterHealth->SetFullHealth(CharacterAttributes->Stats.HitDie);
			CharacterHealth->SetCurrentHealth(CharacterAttributes->Stats.HitDie);
			OnHealthChange();
		}
	}
	
	if (CharacterInventory != nullptr)
	{
		CharacterInventory->Owner = this;
		CharacterInventory->UpdateArmorClass();
		OnStatChange();
	}
	
	/** SpellBook Initialization **/
	if (CharacterAbilityComponent != nullptr)
	{
		CharacterAbilityComponent->Owner = this;
	}
	
	for (auto It = CharacterAttributes->Stats.SpellBook.CreateConstIterator(); It; ++It)
	{
		UBaseAbility* NewAbilityInstance = ArgAbilityStorage->GetAbilityPtr(It.Key());
		if (NewAbilityInstance != nullptr)
		{
			NewAbilityInstance->OwnerCharacter = this;
			AbilityArray.Add(NewAbilityInstance);
			UE_LOG(LogTemp, Warning, TEXT("Spell Added to Abilities"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spell not added because nullptr"));
		}
	}

	/** AcquiredSpells Initialization **/
	for (auto It = CharacterAttributes->Stats.AcquiredSpells.CreateConstIterator(); It; ++It)
	{
		UBaseAbility* NewAbilityInstance = ArgAbilityStorage->GetAbilityPtr(It.Key());
		if (NewAbilityInstance != nullptr)
		{
			NewAbilityInstance->OwnerCharacter = this;
			NewAbilityInstance->SpellCastingAbility = It.Value();
			AcquiredAbilityArray.Add(NewAbilityInstance);
			UE_LOG(LogTemp, Warning, TEXT("Spell Added to AcquiredAbilities"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spell not added because nullptr"));
		}
}
	/** Inventory Initialization **/
	if (CharacterInventory != nullptr)
	{
		CharacterInventory->Owner = this;
		for (auto It = CharacterAttributes->Stats.Inventory.CreateConstIterator(); It; ++It)
		{
			UBaseItem* NewItemInstance = ArgItemStorage->GetItemPtr(It.Key());
			if (NewItemInstance != nullptr)
			{
				CharacterInventory->AddItemToInventory(NewItemInstance);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BaseCharacter - InitializeCharacter(): Item query from storage returned nullptr."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseCharacter - InitializeCharacter(): Character inventory is not initialized."));
	}
	OnConstructSpellBook();
}

void ABaseCharacter::InitializeEnemyCharacter(AGrid* ArgGrid, AIndicatorActor* ArgIndicator, AAbilityStorage* ArgAbilityStorage, AItemStorage* ArgItemStorage)
{
	/** Initializing utility actors **/
	Grid = ArgGrid;
	Indicator = ArgIndicator;

	/** Passing Grid information to pathfinder **/
	Pathfinder->Rows = Grid->Rows;
	Pathfinder->Columns = Grid->Columns;

	/** Initializing Health and Attributes **/
	if (CharacterAttributes != nullptr && CharacterHealth != nullptr)
	{
		CharacterHealth->Owner = this;
		
		if (CharacterHealth)
		{
			CharacterHealth->SetFullHealth(120);
			CharacterHealth->SetCurrentHealth(120);
			OnHealthChange();
		}
	}

	if (CharacterInventory != nullptr)
	{
		CharacterInventory->Owner = this;
		CharacterInventory->UpdateArmorClass();
		OnStatChange();
	}

	/** SpellBook Initialization **/
	if (CharacterAbilityComponent != nullptr)
	{
		CharacterAbilityComponent->Owner = this;
	}

	for (auto It = CharacterAttributes->Stats.SpellBook.CreateConstIterator(); It; ++It)
	{
		UBaseAbility* NewAbilityInstance = ArgAbilityStorage->GetAbilityPtr(It.Key());
		if (NewAbilityInstance != nullptr)
		{
			NewAbilityInstance->OwnerCharacter = this;
			AbilityArray.Add(NewAbilityInstance);
			UE_LOG(LogTemp, Warning, TEXT("Spell Added to Abilities"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spell not added because nullptr"));
		}
	}

	/** AcquiredSpells Initialization **/
	for (auto It = CharacterAttributes->Stats.AcquiredSpells.CreateConstIterator(); It; ++It)
	{
		UBaseAbility* NewAbilityInstance = ArgAbilityStorage->GetAbilityPtr(It.Key());
		if (NewAbilityInstance != nullptr)
		{
			NewAbilityInstance->OwnerCharacter = this;
			NewAbilityInstance->SpellCastingAbility = It.Value();
			AcquiredAbilityArray.Add(NewAbilityInstance);
			UE_LOG(LogTemp, Warning, TEXT("Spell Added to AcquiredAbilities"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spell not added because nullptr"));
		}
	}
	
	/** Inventory Initialization **/
	if (CharacterInventory != nullptr)
	{
		CharacterInventory->Owner = this;
		for (auto It = CharacterAttributes->Stats.Inventory.CreateConstIterator(); It; ++It)
		{
			UBaseItem* NewItemInstance = ArgItemStorage->GetItemPtr(It.Key());
			if (NewItemInstance != nullptr)
			{
				CharacterInventory->AddItemToInventory(NewItemInstance);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BaseCharacter - InitializeCharacter(): Item query from storage returned nullptr."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseCharacter - InitializeCharacter(): Character inventory is not initialized."));
	}
}

int32 ABaseCharacter::GetRow() const
{
	int32 Index = (((GetActorLocation().X - 50) / Grid->fieldSize) * Grid->Columns) + ((GetActorLocation().Y - 50) / Grid->fieldSize);
	int32 row = Index / Grid->Columns;
	return row;
}

int32 ABaseCharacter::GetColumn() const
{
	int32 Index = (((GetActorLocation().X - 50) / Grid->fieldSize) * Grid->Columns) + ((GetActorLocation().Y - 50) / Grid->fieldSize);
	int32 column = Index % Grid->Columns;
	return column;
}

UHealthComponent* ABaseCharacter::GetCharacterHealth() const
{
	if (CharacterHealth)
	{
		return CharacterHealth;
	}
	else
	{
		return nullptr;
	}
}

UAttributesComponent* ABaseCharacter::GetCharacterAttributes() const
{
	if (CharacterAttributes)
	{
		return CharacterAttributes;
	}
	else
	{
		return nullptr;
	}
}

/** Implementations are necessary for **/
void ABaseCharacter::OnHealthChange_Implementation()
{
}
void ABaseCharacter::OnStatChange_Implementation()
{
}
void ABaseCharacter::OnPathfinding_Implementation(const FTransform transform)
{
}
void ABaseCharacter::CleanupPathfinding_Implementation()
{
}
void ABaseCharacter::OnAbilityAim_Implementation(const FTransform transform)
{
}
void ABaseCharacter::CleanupAbilityIndicators_Implementation()
{
}
void ABaseCharacter::OnAttackEnemy_Implementation(const ABaseCharacter* attackedCharacter) 
{
}
void ABaseCharacter::OnBeingAttacked_Implementation(const ABaseCharacter* attackingCharacter)
{
}
void ABaseCharacter::OnConstructSpellBook_Implementation() {}
