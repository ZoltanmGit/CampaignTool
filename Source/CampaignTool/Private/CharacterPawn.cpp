// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPawn.h"
#include "../Public/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACharacterPawn::ACharacterPawn()
{
	//Create components 

	
	CharacterDisplay = CreateDefaultSubobject<UWidgetComponent>(TEXT("CharacterDisplay"));
	RootComponent = CharacterDisplay; //Set the Widget as the RootComponent
	CharacterDisplay->SetWorldRotation(FRotator(90.0f, 0.0f, 0.0f));

	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(RootComponent);

	CharacterHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("CharacterHealth"));

	// Subscribe to HandleTakeDamage to OnTakeAnyDamage event
	OnTakeAnyDamage.AddDynamic(this, &ACharacterPawn::HandleTakeDamage);
}

// Called when the game starts or when spawned
void ACharacterPawn::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterHealth != nullptr)
	{
		CharacterHealth->SetCurrentHealth(CharacterHealth->GetFullHealth());
		OnHealthChange();
	}
}

// Called to bind functionality to input
void ACharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TestAction", IE_Pressed, this, &ACharacterPawn::HandleTestAction);
}

void ACharacterPawn::HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (CharacterHealth != nullptr)
	{
		CharacterHealth->HandleTakeDamage(Damage);
		OnHealthChange();
		UE_LOG(LogTemp, Warning, TEXT("Character's Health: %f"), CharacterHealth->GetCurrentHealth());
	}
}
//Events
void ACharacterPawn::OnHealthChange_Implementation() {}
void ACharacterPawn::HandleTestAction()
{
	UGameplayStatics::ApplyDamage(this, 20, nullptr, this, nullptr);
}

