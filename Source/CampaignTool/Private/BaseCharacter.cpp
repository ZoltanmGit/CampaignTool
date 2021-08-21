// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "../Public/HealthComponent.h"
#include "../Public/AttributesComponent.h"
#include "../Public/PathfinderComponent.h"
#include "../Public/Grid.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Mesh
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	CharacterMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CharacterMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	CharacterMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // I Set this so 
	RootComponent = CharacterMesh; //Set Mesh as the RootComponent

	//Health and Attributes
	CharacterHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("CharacterHealth"));
	CharacterAttributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("CharacterAttributes"));
	Pathfinder = CreateDefaultSubobject<UPathfinderComponent>(TEXT("PathFinder"));


	Grid = nullptr;
	bIsCharacterActive = false;

	// Subscribe to HandleTakeDamage to OnTakeAnyDamage event
	OnTakeAnyDamage.AddDynamic(this, &ABaseCharacter::HandleTakeDamage);
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
	Grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	if (Grid)
	{
		Pathfinder->MapSize = Grid->MapSize;

		UE_LOG(LogTemp, Warning, TEXT("Found grid."));
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("Didn't find Grid."));
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
	CharacterHealth->HandleTakeDamage(Damage);
	OnHealthChange();
}

void ABaseCharacter::BeginTurn()
{
	bIsCharacterActive = true;
	bCanMove = true;
	bCanAct = true;
	CurrentSpeed = CharacterAttributes->Stats.Speed / 5.0f; //We divide by five because 1 square is supposed to be 5ft so 25ft is 5 tiles
	RefreshPathfinding();
}

void ABaseCharacter::EndTurn()
{
	bIsCharacterActive = false;
}

void ABaseCharacter::ChangeLocation(FVector newLocation)
{
	
	bool bValidMovement = false;
	FTransform transform;
	transform.SetLocation(newLocation);
	int32 index;
	Grid->GetTilePropertiesFromTransform(transform, index);
	if (newLocation != CharacterLocation && Pathfinder->ValidIndexMap.Contains(index))
	{
		CleanupPathfinding(); //PLACEHOLDER 
		CurrentSpeed = CurrentSpeed - *Pathfinder->ValidIndexMap.Find(index);
		UE_LOG(LogTemp, Warning, TEXT("Speed is %f"), CurrentSpeed);
		Grid->GridDataArray[index].bIsOccupied = true;
		transform.SetLocation(this->GetActorLocation());
		Grid->GetTilePropertiesFromTransform(transform, index);
		Grid->GridDataArray[index].bIsOccupied = false;
		CharacterLocation = newLocation;

	}
}

void ABaseCharacter::RefreshPathfinding()
{
	int32 index;
	FTileProperties tile = Grid->GetTilePropertiesFromTransform(this->GetActorTransform(), index);
	Pathfinder->GetValidMovementIndexes(tile.Row, tile.Column, CurrentSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Pathfinding is refreshed"));


	//PLACEHOLDER
	for (TPair<int32, float>& Kvp : Pathfinder->ValidIndexMap) //Source: https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/#range-basedfor
	{
		float i;
		float j;
		i = Kvp.Key / Grid->MapSize;
		j = Kvp.Key % Grid->MapSize;
		FTransform Transform;
		Transform.SetLocation(FVector((i * Grid->fieldSize) + (Grid->fieldSize / 2), (j * Grid->fieldSize) + (Grid->fieldSize / 2), 0.0f));
		OnPathfinding(Transform);
	}
}

void ABaseCharacter::InitializeCharacter(FCharacterStruct Character)
{
	if (CharacterAttributes)
	{
		CharacterAttributes->InitComponent(Character);
		if (CharacterHealth)
		{

		}
	}
}

UHealthComponent* ABaseCharacter::GetCharacterHealth() const
{
	return CharacterHealth;
}

UAttributesComponent* ABaseCharacter::GetCharacterAttributes() const
{
	return CharacterAttributes;
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