// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../Public/HealthComponent.h"
#include "../Public/AttributesComponent.h"
#include "../Public/PathfinderComponent.h"
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
	TargetedCharacter = nullptr;
	TargetedTile = nullptr;
	bIsRightMouseDown = false;
	bCastMouseLineTrace = true;
	bIsValidMove = true;
	bIsPossessed = false;
	PlayerCursorState = CursorState::OverGameWorld;
	CharacterType = CharacterType::Ally;
}
/// <summary>
/// Beginplay is called when the game starts or when the actor is spawned
/// </summary>
void APlayerCharacter::BeginPlay()
{
	DefaultController = Cast<APlayerController>(GetController());
	if (DefaultController != nullptr)
	{
		DefaultController->bEnableClickEvents = true;
		DefaultController->bEnableMouseOverEvents = true;
		DefaultController->bShowMouseCursor = true;
	}
	CharacterSpringArm->SetWorldLocation(FVector(50.0f, 50.0f, 50.0f));
	CharacterLocation = GetActorLocation();

	Super::BeginPlay();
}
/// <summary>
/// Sets up the player inputs
/// </summary>
/// <param name="PlayerInputComponent">The component through which the setup is handled</param>
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
/// <summary>
/// The Tick function gets called every frame
/// </summary>
/// <param name="DeltaTime">The DeltaTime for this tick</param>
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Line traces and Selection
	if ((DefaultController != nullptr && bCastMouseLineTrace && PlayerCursorState == CursorState::OverGameWorld && bIsPossessed))
	{

		if (DefaultController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, UnderCursorHitResult))
		{
			if (UnderCursorHitResult.IsValidBlockingHit())
			{
				if (Grid == UnderCursorHitResult.Actor) //Check if the grid is targeted
				{
					if (TargetedCharacter != nullptr) //If the grid is targeted then the Target Character should be a null pointer
					{
						TargetedCharacter = nullptr;
					}
					if (TargetedTile != UnderCursorHitResult.Component) // Only cast if it's not already targeted
					{
						TargetedTile = Cast<UHierarchicalInstancedStaticMeshComponent>(UnderCursorHitResult.Component);
					}

					FTransform TileTransform;
					TargetedTile->GetInstanceTransform(UnderCursorHitResult.Item, TileTransform, true); //Then its transform
					int32 tileIndex;
					FTileProperties hitTileProperties = Grid->GetTilePropertiesFromTransform(TileTransform, tileIndex);
					
					CursorLocation = TileTransform.GetLocation();
					//DEBUG
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-2, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile Location: %s"), *TileTransform.GetLocation().ToString()));
						GEngine->AddOnScreenDebugMessage(-3, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile TerrainType: %i"), hitTileProperties.TerrainType));
						GEngine->AddOnScreenDebugMessage(-4, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile LightType: %i"), hitTileProperties.LightType));
						GEngine->AddOnScreenDebugMessage(-5, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile Row: %i"), hitTileProperties.Row));
						GEngine->AddOnScreenDebugMessage(-6, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile Column: %i"), hitTileProperties.Column));
					}
				}
				else //If the grid isn't targeted then we check for characters
				{
					if (TargetedTile != nullptr) //If a character is targeted then a tile should not be selected
					{
						TargetedTile = nullptr;
					}
					if (TargetedCharacter != UnderCursorHitResult.Actor) //Only cast if it's not already targeted
					{
						TargetedCharacter = Cast<ABaseCharacter>(UnderCursorHitResult.Actor);
					}
					if (TargetedCharacter != nullptr)
					{
						if (GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-2, 0.0, FColor::Emerald, FString::Printf(TEXT("Character is selected")));
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Character selection failed."));
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid Block. (You shouldn't see this :O )"));
			}
		}
		else
		{
			if (TargetedCharacter != nullptr)
			{
				TargetedCharacter = nullptr;
			}
			if (TargetedTile != nullptr)
			{
				TargetedTile = nullptr;
			}
			GEngine->AddOnScreenDebugMessage(-2, 0.0, FColor::Emerald, TEXT("Trace didn't hit"));
			CursorLocation = this->GetActorLocation();
		}
	}
}
/// <summary>
/// Test action that is bound to the "T" letter
/// </summary>
void APlayerCharacter::HandleTestAction()
{
	//UGameplayStatics::ApplyDamage(this, 5, nullptr, this, nullptr);

	//CharacterSpringArm->SetWorldLocation(this->GetActorLocation());
	this->BeginTurn();
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
	//OnPathfinding();
}
/// <summary>
/// Moves the camera in a forward and backward direction relative to its current rotation
/// </summary>
/// <param name="value">The value that drives the movement</param>
void APlayerCharacter::CameraForward(float value)
{
	if (value != 0 && CharacterCamera != nullptr && CharacterSpringArm != nullptr)
	{
		FVector newVector = FVector(CharacterSpringArm->GetForwardVector().X, CharacterSpringArm->GetForwardVector().Y, 0.0f);
		CharacterSpringArm->SetWorldLocation(CharacterSpringArm->GetComponentLocation() - (newVector * (value * 10)));
	}
}
/// <summary>
/// Moves the camera in the right and left direction relative to its current rotation
/// </summary>
/// <param name="value">The value that drives the movement</param>
void APlayerCharacter::CameraRight(float value)
{
	if (value != 0 && CharacterCamera != nullptr && CharacterSpringArm != nullptr)
	{
		FVector newVector = FVector(CharacterSpringArm->GetRightVector().X, CharacterSpringArm->GetRightVector().Y, 0.0f);
		CharacterSpringArm->SetWorldLocation(CharacterSpringArm->GetComponentLocation() - (newVector * (-1 * value * 10)));

		//UE_LOG(LogTemp, Warning, TEXT("CameraRight() called"));
	}
}
/// <summary>
/// Turns the Actor's Spring Arm and with it the camera that is attached to it
/// </summary>
/// <param name="value">The value that drives the rotation</param>
void APlayerCharacter::CameraTurn(float value)
{
	if (value != 0 && CharacterCamera != nullptr && CharacterSpringArm != nullptr && bIsRightMouseDown)
	{
		CharacterSpringArm->SetRelativeRotation(FRotator(CharacterSpringArm->GetRelativeRotation().Pitch, CharacterSpringArm->GetRelativeRotation().Yaw + value, CharacterSpringArm->GetRelativeRotation().Roll));

		//UE_LOG(LogTemp, Warning, TEXT("CameraTurn() called"));
	}
}
/// <summary>
/// Handles the behaviour of the right mouse button being pressed down
/// </summary>
void APlayerCharacter::HandleRMBPress()
{
	if (!bIsRightMouseDown && PlayerCursorState == CursorState::OverGameWorld)
	{
		if (TargetedCharacter != nullptr)
		{
			TargetedCharacter = nullptr;
		}
		if (TargetedTile != nullptr)
		{
			TargetedTile = nullptr;
		}

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
/// <summary>
/// Handles the behaviour of the right mouse button being released
/// </summary>
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
/// <summary>
/// Handles the behaviour of the Left Mouse Button being pressed down
/// </summary>
void APlayerCharacter::HandleLMBPress()
{
	if (TargetedTile != nullptr && bIsValidMove)
	{
		CharacterLocation = FVector(CursorLocation.X, CursorLocation.Y, 50.0f);
	}
	else if (TargetedCharacter != nullptr)
	{
		if (TargetedCharacter != this)
		{
			ChangePossession(TargetedCharacter);
		}
	}
}
/// <summary>
/// Changes the possession of the playerController and clears some values for the previous possession
/// </summary>
/// <param name="newCharacter">The character which is next to be possessed</param>
void APlayerCharacter::ChangePossession(ABaseCharacter* newCharacter)
{
	APlayerCharacter* newPlayerCharacter = Cast<APlayerCharacter>(newCharacter);
	newPlayerCharacter->CharacterSpringArm->SetWorldRotation(this->CharacterSpringArm->GetComponentRotation());
	newPlayerCharacter->CharacterSpringArm->SetWorldLocation(newPlayerCharacter->GetActorLocation());
	newPlayerCharacter->DefaultController = this->DefaultController;
	this->DefaultController->UnPossess();
	this->bIsPossessed = false;
	this->DefaultController->Possess(newPlayerCharacter);
	newPlayerCharacter->bIsPossessed = true;

	//Cleanup this pawn
	this->CharacterSpringArm->SetWorldLocation(this->GetActorLocation());
	this->DefaultController = nullptr;
}

void APlayerCharacter::OnPathfinding_Implementation(const FTransform transform)
{

}