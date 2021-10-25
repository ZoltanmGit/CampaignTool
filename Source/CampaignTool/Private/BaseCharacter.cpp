// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/SplineComponent.h"
#include "../Public/HealthComponent.h"
#include "../Public/AbilityComponent.h"
#include "../Public/AttributesComponent.h"
#include "../Public/PathfinderComponent.h"
#include "../Public/MoverComponent.h"
#include "../Public/Grid.h"
#include "IndicatorActor.h"
#include "Kismet/GameplayStatics.h"

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
	CharacterMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // I Set this so 
	RootComponent = CharacterMesh; //Set Mesh as the RootComponent
	
	/** Movement **/
	Mover = CreateDefaultSubobject<UMoverComponent>(TEXT("MoverComponent"));
	Pathfinder = CreateDefaultSubobject<UPathfinderComponent>(TEXT("PathFinder"));
	
	
	/** General Components **/
	// Health
	CharacterHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("CharacterHealth"));
	CharacterHealth->Owner = this;
	
	// Attributes
	CharacterAttributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("CharacterAttributes"));
	
	
	
	// Ability Component
	CharacterAbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
	CharacterAbilityComponent->Owner = this;

	Grid = nullptr;

	// Subscribe to HandleTakeDamage to OnTakeAnyDamage event
	OnTakeAnyDamage.AddDynamic(this, &ABaseCharacter::HandleTakeDamage); //Might be redundant soon
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CharacterHealth != nullptr)
	{
		CharacterHealth->SetCurrentHealth(CharacterHealth->GetFullHealth());
		OnHealthChange();
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

void ABaseCharacter::HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (CharacterHealth)
	{
		CharacterHealth->HandleTakeDamage(Damage);
		OnHealthChange();
	}
}

void ABaseCharacter::BeginTurn()
{
	if (Grid && CharacterHealth && CharacterAttributes && Pathfinder && Mover && !bCanAct && !bCanMove && !bIsActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Components are valid at BeginTurn"));
		bCanAct = true;
		bCanMove = true;
		bIsActive = true;
		CurrentSpeed = CharacterAttributes->Stats.Speed / 5.0f;
		RefreshPathfinding();
	}
}

void ABaseCharacter::EndTurn()
{
	bCanAct = false;
	bCanMove = false;
	bIsActive = false;
	CleanupPathfinding();
	UE_LOG(LogTemp, Warning, TEXT("Turn Ended"));
}

void ABaseCharacter::ChangeLocation(FVector newLocation)
{
	if (Grid && Pathfinder && Mover && bCanMove)
	{
		bCanMove = false;
		FTransform TempTransform;
		TempTransform.SetLocation(newLocation);
		int32 index;
		Grid->GetTilePropertiesFromTransform(TempTransform, index);
		if (Pathfinder->ValidIndexMap.Contains(index) && Mover->Timeline)
		{
			float minusSpeed = *Pathfinder->ValidIndexMap.Find(index);
			CurrentSpeed = CurrentSpeed - minusSpeed;
			UE_LOG(LogTemp, Warning, TEXT("Speed is %f"), CurrentSpeed);


			int32 CurrentIndex;
			FTileProperties CurrentTileData = Grid->GetTilePropertiesFromTransform(this->GetActorTransform(), CurrentIndex);
			Grid->GridDataArray[CurrentIndex].bIsOccupied = false;
			Grid->GridDataArray[CurrentIndex].ActorOnTile = nullptr;

			int32 NewTileIndex;
			FTileProperties NewLocationTileData = Grid->GetTilePropertiesFromTransform(TempTransform, NewTileIndex);
			Grid->GridDataArray[NewTileIndex].bIsOccupied = true;
			Grid->GridDataArray[NewTileIndex].ActorOnTile = this;
			
			UE_LOG(LogTemp, Warning, TEXT("Speed: %f"),(1/minusSpeed)*4);
			Mover->Timeline->SetPlayRate((1/minusSpeed)*4);
			Mover->MoveCharacter(newLocation);
			CharacterLocation = newLocation;
		}
		else
		{
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


		//PLACEHOLDER
		for (TPair<int32, float>& Kvp : Pathfinder->ValidIndexMap) //Source: https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/#range-basedfor
		{
			float i;
			float j;
			i = Kvp.Key / Grid->Columns;
			j = Kvp.Key % Grid->Columns;
			FTransform Transform;
			Transform.SetLocation(FVector((i * Grid->fieldSize) + (Grid->fieldSize / 2), (j * Grid->fieldSize) + (Grid->fieldSize / 2), 5.0f));
			OnPathfinding(Transform);
		}
		UE_LOG(LogTemp, Warning, TEXT("Pathfinding refreshed"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pathfinder is nullptr"));
	}
}

void ABaseCharacter::InitializeCharacter(FCharacterStruct Character, AGrid* ArgGrid, AIndicatorActor* ArgIndicator)
{
	Grid = ArgGrid;
	Indicator = ArgIndicator;
	Pathfinder->Rows = Grid->Rows;
	Pathfinder->Columns = Grid->Columns;
	if (CharacterAttributes)
	{
		CharacterAttributes->InitComponent(Character);
		if (CharacterHealth)
		{
			//TODO
		}
	}
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

void ABaseCharacter::OnHealthChange_Implementation()
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