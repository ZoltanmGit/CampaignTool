// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../Public/HealthComponent.h"
#include "../Public/AttributesComponent.h"
#include "../Public/Grid.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Camera and SpringArm
	CharacterSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArm"));
	CharacterSpringArm->TargetArmLength = 700.0f;
	CharacterSpringArm->bEnableCameraLag = true;
	CharacterSpringArm->SetRelativeRotation(FRotator(-40.0f, 0.0f, 0.0f));
	CharacterSpringArm->SetUsingAbsoluteLocation(true);
	//CharacterSpringArm->SetupAttachment(RootComponent);

	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CharacterCamera->SetupAttachment(CharacterSpringArm, USpringArmComponent::SocketName); //Attach the camera to the SpringArmComponent

	//Initialize values
	bIsRightMouseDown = false;
	bCastMouseLineTrace = true;
	PlayerCursorState = CursorState::OverGameWorld;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultController = Cast<APlayerController>(GetController());
	if (DefaultController != nullptr)
	{
		DefaultController->bEnableClickEvents = true;
		DefaultController->bEnableMouseOverEvents = true;
		DefaultController->bShowMouseCursor = true;
	}
	SetActorLocation(FVector(50.0f, 50.0f, 50.0f));
	CharacterSpringArm->SetWorldLocation(FVector(50.0f, 50.0f, 50.0f));
	CharacterLocation = GetActorLocation();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Axis
	PlayerInputComponent->BindAxis("CameraRight", this, &APlayerCharacter::CameraRight);
	PlayerInputComponent->BindAxis("CameraForward", this, &APlayerCharacter::CameraForward);
	PlayerInputComponent->BindAxis("CameraTurn", this, &APlayerCharacter::CameraTurn);
	//Action
	PlayerInputComponent->BindAction("TestAction", IE_Pressed, this, &APlayerCharacter::HandleTestAction);
	PlayerInputComponent->BindAction("RMBAction", IE_Pressed, this, &APlayerCharacter::HandleRMBPress);
	PlayerInputComponent->BindAction("RMBAction", IE_Released, this, &APlayerCharacter::HandleRMBRelease);
	PlayerInputComponent->BindAction("LMBAction", IE_Pressed, this, &APlayerCharacter::HandleLMBPress);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((DefaultController != nullptr && bCastMouseLineTrace && PlayerCursorState == CursorState::OverGameWorld))
	{

		if (DefaultController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, UnderCursorHitResult))
		{
			if (UnderCursorHitResult.IsValidBlockingHit())
			{
				if (Grid == UnderCursorHitResult.Actor)
				{
					UHierarchicalInstancedStaticMeshComponent* hitHISMC = Cast<UHierarchicalInstancedStaticMeshComponent>(UnderCursorHitResult.Component);

					FTransform TileTransform;
					hitHISMC->GetInstanceTransform(UnderCursorHitResult.Item, TileTransform, true);
					CursorLocation = TileTransform.GetLocation();

					int32 tileIndex;
					FTileProperties hitTileProperties = Grid->GetTilePropertiesFromTransform(TileTransform, tileIndex);

					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-2, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile Location: %s"), *TileTransform.GetLocation().ToString()));
						GEngine->AddOnScreenDebugMessage(-3, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile TerrainType: %i"), hitTileProperties.TerrainType));
						GEngine->AddOnScreenDebugMessage(-4, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile LightType: %i"), hitTileProperties.LightType));
						GEngine->AddOnScreenDebugMessage(-5, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile Row: %i"), hitTileProperties.Row));
						GEngine->AddOnScreenDebugMessage(-6, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile Column: %i"), hitTileProperties.Column));

					}
				}
			}
		}
	}
	if (this->GetActorLocation() != CharacterLocation)
	{
		
		this->SetActorLocation(FVector(FMath::VInterpTo(this->GetActorLocation(), CharacterLocation, DeltaTime, 10.0f).X, FMath::VInterpTo(this->GetActorLocation(), CharacterLocation, DeltaTime, 10.0f).Y,50.0f));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-7, 0.0, FColor::Emerald,TEXT("Interpolating..."));
		}
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-8, 0.0, FColor::Red, FString::Printf(TEXT("ActorLocation: %s"), *GetActorLocation().ToString()));
		GEngine->AddOnScreenDebugMessage(-9, 0.0, FColor::Red, FString::Printf(TEXT("CharacterLocation: %s"), *CharacterLocation.ToString()));
	}

}

void APlayerCharacter::HandleTestAction()
{
	UGameplayStatics::ApplyDamage(this, 5, nullptr, this, nullptr);

	CharacterSpringArm->SetWorldLocation(this->GetActorLocation());
}

void APlayerCharacter::CameraForward(float value)
{
	if (value != 0 && CharacterCamera != nullptr && CharacterSpringArm != nullptr)
	{
		FVector newVector = FVector(CharacterSpringArm->GetForwardVector().X, CharacterSpringArm->GetForwardVector().Y, 0.0f);
		CharacterSpringArm->SetWorldLocation(CharacterSpringArm->GetComponentLocation() - (newVector * (value * 10)));

		//UE_LOG(LogTemp, Warning, TEXT("CameraForward() called"));
	}
}

void APlayerCharacter::CameraRight(float value)
{
	if (value != 0 && CharacterCamera != nullptr && CharacterSpringArm != nullptr)
	{
		FVector newVector = FVector(CharacterSpringArm->GetRightVector().X, CharacterSpringArm->GetRightVector().Y, 0.0f);
		CharacterSpringArm->SetWorldLocation(CharacterSpringArm->GetComponentLocation() - (newVector * (-1 * value * 10)));

		//UE_LOG(LogTemp, Warning, TEXT("CameraRight() called"));
	}
}

void APlayerCharacter::CameraTurn(float value)
{
	if (value != 0 && CharacterCamera != nullptr && CharacterSpringArm != nullptr && bIsRightMouseDown)
	{
		CharacterSpringArm->SetRelativeRotation(FRotator(CharacterSpringArm->GetRelativeRotation().Pitch, CharacterSpringArm->GetRelativeRotation().Yaw + value, CharacterSpringArm->GetRelativeRotation().Roll));

		//UE_LOG(LogTemp, Warning, TEXT("CameraTurn() called"));
	}
}

void APlayerCharacter::HandleRMBPress()
{
	if (!bIsRightMouseDown && PlayerCursorState == CursorState::OverGameWorld)
	{
		bIsRightMouseDown = true;
		PlayerCursorState = CursorState::Turning;
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

void APlayerCharacter::HandleRMBRelease()
{
	if (bIsRightMouseDown && PlayerCursorState == CursorState::Turning)
	{
		bIsRightMouseDown = false;
		PlayerCursorState = CursorState::OverGameWorld;

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

void APlayerCharacter::HandleLMBPress()
{
	UE_LOG(LogTemp, Warning, TEXT("LMBPresse Called"));
	CharacterLocation = FVector(CursorLocation.X,CursorLocation.Y,50.0f);
}
