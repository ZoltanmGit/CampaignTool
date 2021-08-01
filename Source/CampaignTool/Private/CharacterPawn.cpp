// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPawn.h"
#include "../Public/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Sets default values
ACharacterPawn::ACharacterPawn()
{
	//Mesh
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	RootComponent = CharacterMesh; //Set Mesh as the RootComponent

	//Camera and SpringArm
	CharacterSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArm"));
	CharacterSpringArm->TargetArmLength = 600.0f;
	CharacterSpringArm->bEnableCameraLag = true;
	CharacterSpringArm->SetRelativeRotation(FRotator(-40.0f, 0.0f, 0.0f));
	CharacterSpringArm->SetupAttachment(RootComponent);

	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CharacterCamera->SetupAttachment(CharacterSpringArm, USpringArmComponent::SocketName); //Attach the camera to the SpringArmComponent


	//Health and Attributes
	CharacterHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("CharacterHealth"));

	//Initialize values
	bIsRightMouseDown = false;
	
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

	DefaultController = Cast<APlayerController>(GetController());
	if (DefaultController != nullptr)
	{
		DefaultController->bEnableClickEvents = true;
		DefaultController->bEnableMouseOverEvents = true;
		DefaultController->bShowMouseCursor = true;
	}
}

// Called to bind functionality to input
void ACharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Axis
	PlayerInputComponent->BindAxis("CameraRight", this, &ACharacterPawn::CameraRight);
	PlayerInputComponent->BindAxis("CameraForward", this, &ACharacterPawn::CameraForward);
	PlayerInputComponent->BindAxis("CameraTurn", this, &ACharacterPawn::CameraTurn);
	//Action
	PlayerInputComponent->BindAction("TestAction", IE_Pressed, this, &ACharacterPawn::HandleTestAction);
	PlayerInputComponent->BindAction("RMBAction", IE_Pressed, this, &ACharacterPawn::HandleRMBPress);
	PlayerInputComponent->BindAction("RMBAction", IE_Released, this, &ACharacterPawn::HandleRMBRelease);
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

void ACharacterPawn::CameraForward(float value)
{
	if (value != 0 && CharacterCamera != nullptr && CharacterSpringArm != nullptr)
	{
		FVector newVector = FVector(CharacterSpringArm->GetForwardVector().X, CharacterSpringArm->GetForwardVector().Y, 0.0f);
		CharacterSpringArm->SetWorldLocation(CharacterSpringArm->GetComponentLocation() - (newVector * (value*10)));
		
		//UE_LOG(LogTemp, Warning, TEXT("CameraForward() called"));
	}
}

void ACharacterPawn::CameraRight(float value)
{
	if (value != 0 && CharacterCamera != nullptr && CharacterSpringArm != nullptr)
	{
		FVector newVector = FVector(CharacterSpringArm->GetRightVector().X, CharacterSpringArm->GetRightVector().Y, 0.0f);
		CharacterSpringArm->SetWorldLocation(CharacterSpringArm->GetComponentLocation() - (newVector * (-1 * value * 10)));
		
		//UE_LOG(LogTemp, Warning, TEXT("CameraRight() called"));
	}
}

void ACharacterPawn::CameraTurn(float value)
{
	if (value != 0 && CharacterCamera != nullptr && CharacterSpringArm != nullptr && bIsRightMouseDown)
	{
		CharacterSpringArm->SetRelativeRotation(FRotator(CharacterSpringArm->GetRelativeRotation().Pitch, CharacterSpringArm->GetRelativeRotation().Yaw + value, CharacterSpringArm->GetRelativeRotation().Roll));
		
		//UE_LOG(LogTemp, Warning, TEXT("CameraTurn() called"));
	}
}

void ACharacterPawn::HandleRMBPress()
{
	if (!bIsRightMouseDown)
	{
		bIsRightMouseDown = true;
		if (DefaultController)
		{
			DefaultController->GetMousePosition(savedMousePosition.X, savedMousePosition.Y);
			DefaultController->bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			DefaultController->SetInputMode(InputMode);
			DefaultController->bEnableClickEvents = false;
			DefaultController->bEnableMouseOverEvents = false;
		}
	}
}
void ACharacterPawn::HandleRMBRelease()
{
	if (bIsRightMouseDown)
	{
		bIsRightMouseDown = false;
		if (DefaultController)
		{
			DefaultController->SetMouseLocation(savedMousePosition.X, savedMousePosition.Y);
			DefaultController->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			DefaultController->SetInputMode(InputMode);
			DefaultController->bEnableClickEvents = true;
			DefaultController->bEnableMouseOverEvents = true;
		}
	}
}

