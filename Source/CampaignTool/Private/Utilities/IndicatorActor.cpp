// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/IndicatorActor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Constructor - Sets default values
AIndicatorActor::AIndicatorActor()
{
	PrimaryActorTick.bCanEverTick = false;
	/** Root Component **/
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("IndicatorRoot"));
	SetRootComponent(RootScene);
	RootScene->SetMobility(EComponentMobility::Static);
	/** Init Static Meshes **/
	MovementIndicatorHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("MovementHISMC"));
	MovementIndicatorHISMC->SetupAttachment(RootScene);
	MovementIndicatorHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision); //Does not react to physics, only overlaps, traces , etc...
	MovementIndicatorHISMC->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore); // Only blocks the selection channel
	MovementIndicatorHISMC->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Overlap); //Overlap so the camera springarm doesn't shorten itself with strange collision
	MovementIndicatorHISMC->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	MovementIndicatorHISMC->SetMobility(EComponentMobility::Static);
	MovementIndicatorHISMC->bCastContactShadow = false;
	MovementIndicatorHISMC->CastShadow = false;

	AbilityIndicatorHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("AbilityHISMC"));
	AbilityIndicatorHISMC->SetupAttachment(RootScene);
	AbilityIndicatorHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Has no collision whatsoever
	AbilityIndicatorHISMC->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore); // Ignores player selection trace
	AbilityIndicatorHISMC->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Overlap); // Overlap so the camera springarm doesn't shorten itself with strange collision
	AbilityIndicatorHISMC->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap); // Overlaps with Pawns
	AbilityIndicatorHISMC->SetMobility(EComponentMobility::Static);
	AbilityIndicatorHISMC->bCastContactShadow = false;
	AbilityIndicatorHISMC->CastShadow = false;
}

// Called when the game starts or when spawned
void AIndicatorActor::BeginPlay()
{
	Super::BeginPlay();
}
// Spawns an instance of a mesh of a given type at the location
void AIndicatorActor::SpawnIndicator(FVector location, EIndicatorType IndicatorType)
{
	FTransform transform;
	transform.SetLocation(location);
	if (IndicatorType == EIndicatorType::MovementIndicator && MovementIndicatorHISMC != nullptr)
	{
		MovementIndicatorHISMC->AddInstance(transform);
		UE_LOG(LogTemp, Warning, TEXT("MovementHISMC Spawned"));
	}
	else if (IndicatorType == EIndicatorType::AbilityIndicator && AbilityIndicatorHISMC != nullptr)
	{
		AbilityIndicatorHISMC->AddInstance(transform);
	}
}
// Clears spawned meshes
void AIndicatorActor::CleanupIndicators(EIndicatorType IndicatorType)
{
	if (IndicatorType == EIndicatorType::MovementIndicator && MovementIndicatorHISMC != nullptr)
	{
		MovementIndicatorHISMC->ClearInstances();
	}
	else if (IndicatorType == EIndicatorType::AbilityIndicator && AbilityIndicatorHISMC != nullptr)
	{
		AbilityIndicatorHISMC->ClearInstances();
	}
}

