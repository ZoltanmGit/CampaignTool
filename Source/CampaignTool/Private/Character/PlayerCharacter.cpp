
#include "Character/PlayerCharacter.h"
#include "Character/HealthComponent.h"
#include "Character/AttributesComponent.h"
#include "MovementSystem/PathfinderComponent.h"
#include "MovementSystem/MoverComponent.h"
#include "GridSystem/Grid.h"
#include "Utilities/DiceRoller.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystem/AbilityComponent.h"
#include "AbilitySystem/BaseAoeTargetAbility.h"
#include "AbilitySystem/BaseSingleTargetAbility.h"
#include "InventorySystem/BaseItem.h"
#include "InventorySystem/BaseEquippableItem.h"
#include "InventorySystem/BaseArmorItem.h"
#include "InventorySystem/BaseWeaponItem.h"
#include "InventorySystem/BaseShieldItem.h"
#include "InventorySystem/ItemStorage.h"
#include "InventorySystem/CharacterInventoryComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Camera and SpringArm
	CharacterSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArm"));
	CharacterSpringArm->TargetArmLength = 1000.0f;
	CharacterSpringArm->bEnableCameraLag = true;
	CharacterSpringArm->SetRelativeRotation(FRotator(-70.0f, 0.0f, 0.0f));
	CharacterSpringArm->SetUsingAbsoluteLocation(true);

	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CharacterCamera->SetupAttachment(CharacterSpringArm, USpringArmComponent::SocketName); //Attach the camera to the SpringArmComponent
	CharacterCamera->SetFieldOfView(70.0f);

	//Initialize values
	TargetedCharacter = nullptr;
	TargetedTile = nullptr;
	bIsRightMouseDown = false;
	bCastMouseLineTrace = true;
	bIsInventoryCollapsed = true;
	PlayerCursorState = CursorState::OverGameWorld;
	CharacterType = ECharacterType::C_Ally;

	/** DEBUG DELETE LATER **/
	
}

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
	PlayerInputComponent->BindAction("ToogleInventory", IE_Pressed, this, &APlayerCharacter::HandleInventoryToogle);
	PlayerInputComponent->BindAction("EndTurn", IE_Pressed, this, &APlayerCharacter::EndTurn);
	PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &APlayerCharacter::HandleEscape);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Hotkey01", IE_Pressed, this, &APlayerCharacter::HandleHotkey, 0);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Hotkey02", IE_Pressed, this, &APlayerCharacter::HandleHotkey, 1);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Hotkey03", IE_Pressed, this, &APlayerCharacter::HandleHotkey, 2);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Hotkey04", IE_Pressed, this, &APlayerCharacter::HandleHotkey, 3);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Hotkey05", IE_Pressed, this, &APlayerCharacter::HandleHotkey, 4);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Hotkey06", IE_Pressed, this, &APlayerCharacter::HandleHotkey, 5);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Line traces and Selection
	if ((DefaultController != nullptr && bCastMouseLineTrace && PlayerCursorState == CursorState::OverGameWorld && bIsActive))
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
					
					
					if (CursorLocation != TileTransform.GetLocation() && (bCanMove || bIsAimingAbility))
					{
						
						//UE_LOG(LogTemp, Warning, TEXT("New CursorLocation"));
						CursorLocation = TileTransform.GetLocation();

						// if aiming ability
						if (bIsAimingAbility)
						{
							//UE_LOG(LogTemp, Warning, TEXT("______ChangedAim______"));
							CharacterAbilityComponent->HandleTileChange();
						}
						// if aiming movement ..
						else if (bCanMove && bIsAimingMovement)
						{
							if (IsCursorOnValidMovementTile(hitTileProperties.Row,hitTileProperties.Column))
							{
								//UE_LOG(LogTemp, Warning, TEXT("New CursorLocation is Valid Movement"));
								Pathfinder->GetRouteFromIndexes(hitTileProperties.Row, hitTileProperties.Column);
								Mover->RefreshSpline();
							}
							else
							{
								if (Mover->MovementSplineMeshArray.Num() > 0)
								{
									Mover->CleanupSplineMesh();
								}
							}
						}
					}
					//DEBUG
					if (GEngine)
					{

						GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::Emerald, FString::Printf(TEXT("Tile Index: %i"), tileIndex));
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
						CursorLocation = this->GetActorLocation(); // And we set the cursor to the characters location
					}
					if (Mover) //this fires if the cursor moves from a valid tile to directly onto a character
					{
						if (Mover->MovementSplineMeshArray.Num() > 0)
						{
							Mover->CleanupSplineMesh();
						}
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
			if (Mover)
			{
				if (Mover->MovementSplineMeshArray.Num() > 0)
				{
					Mover->CleanupSplineMesh();
				}
			}
			GEngine->AddOnScreenDebugMessage(-2, 0.0, FColor::Emerald, TEXT("Trace didn't hit"));
			CursorLocation = this->GetActorLocation();
		}
	}
}

void APlayerCharacter::ToogleMovement()
{
	if (bCanAct)
	{
		if (CurrentSpeed >= 1 && !bIsAimingMovement)
		{
			if (bIsAimingAbility)
			{
				bIsAimingAbility = false;
				CharacterAbilityComponent->CancelAbility();
			}
			bIsAimingMovement = true;
			RefreshPathfinding();


			FTransform transform;
			transform.SetLocation(CursorLocation);
			int32 DummyIndex;
			FTileProperties tileProp = Grid->GetTilePropertiesFromTransform(transform, DummyIndex);
			if (IsCursorOnValidMovementTile(tileProp.Row, tileProp.Column))
			{
				//UE_LOG(LogTemp, Warning, TEXT("New CursorLocation is Valid Movement"));
				Pathfinder->GetRouteFromIndexes(tileProp.Row, tileProp.Column);
				Mover->RefreshSpline();
			}
			bCanMove = true;
		}
		else if (bIsAimingMovement)
		{
			bIsAimingMovement = false;
			bCanMove = false;
			CleanupPathfinding();
			Mover->CleanupSplineMesh();
		}
	}
}

void APlayerCharacter::HandleTestAction()
{
	
}

void APlayerCharacter::HandleHotkey(int index)
{
	if (AbilityArray.IsValidIndex(index))
	{
		if (bIsAimingMovement)
		{
			bIsAimingMovement = false;
			CleanupPathfinding();
			Mover->CleanupSplineMesh();
		}
		// If we're already aiming the same spell then the aim is canceled
		if (bIsAimingAbility && CharacterAbilityComponent->SelectedAbility == AbilityArray[index])
		{
			CharacterAbilityComponent->CancelAbility();
			bIsAimingAbility = false;
		}
		else if (bIsAimingAbility && CharacterAbilityComponent->SelectedAbility != AbilityArray[index])
		{
			CharacterAbilityComponent->CancelAbility();
			CharacterAbilityComponent->SelectAbility(AbilityArray[index]);
			CharacterAbilityComponent->HandleTileChange();
		}
		else if (!bIsAimingAbility)
		{
			bIsAimingAbility = true;
			CharacterAbilityComponent->SelectAbility(AbilityArray[index]);
			CharacterAbilityComponent->HandleTileChange();
		}
	}
}

void APlayerCharacter::SelectAbility(UBaseAbility* abilityToSelect)
{
	if (bIsAimingMovement)
	{
		bIsAimingMovement = false;
		CleanupPathfinding();
		Mover->CleanupSplineMesh();
	}
	// If we're already aiming the same spell then the aim is canceled
	if (bIsAimingAbility && CharacterAbilityComponent->SelectedAbility == abilityToSelect)
	{
		CharacterAbilityComponent->CancelAbility();
		bIsAimingAbility = false;
	}
	else if (bIsAimingAbility && CharacterAbilityComponent->SelectedAbility != abilityToSelect)
	{
		CharacterAbilityComponent->CancelAbility();
		CharacterAbilityComponent->SelectAbility(abilityToSelect);
		CharacterAbilityComponent->HandleTileChange();
	}
	else if (!bIsAimingAbility)
	{
		bIsAimingAbility = true;
		CharacterAbilityComponent->SelectAbility(abilityToSelect);
		CharacterAbilityComponent->HandleTileChange();
	}
}

void APlayerCharacter::CameraForward(float value)
{
	if (value != 0.0f && CharacterCamera && CharacterSpringArm)
	{
		FVector newLocation = CharacterSpringArm->GetComponentLocation();
		newLocation += FRotator(0.0f, CharacterCamera->GetComponentRotation().Yaw, 0.0f).Vector() * GetWorld()->GetDeltaSeconds() * 700.0f * value;
		CharacterSpringArm->SetWorldLocation(newLocation);
	}
}

void APlayerCharacter::CameraRight(float value)
{
	if (value != 0.0f && CharacterCamera != nullptr && CharacterSpringArm != nullptr)
	{
		FVector newLocation = CharacterSpringArm->GetComponentLocation();
		newLocation += FRotator(0.0f, CharacterCamera->GetComponentRotation().Yaw+90.0f, 0.0f).Vector() * GetWorld()->GetDeltaSeconds() * 700.0f * value;
		CharacterSpringArm->SetWorldLocation(newLocation);
	}
}

void APlayerCharacter::CameraTurn(float value)
{
	if (value != 0 && CharacterCamera != nullptr && CharacterSpringArm != nullptr && bIsRightMouseDown)
	{
		CharacterSpringArm->SetRelativeRotation(FRotator(CharacterSpringArm->GetRelativeRotation().Pitch, CharacterSpringArm->GetRelativeRotation().Yaw + value, CharacterSpringArm->GetRelativeRotation().Roll));
	}
}

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
	// If MovementAiming
	if (TargetedTile != nullptr && bCanMove && Mover && !bIsAimingAbility)
	{
		this->ChangeLocation(FVector(CursorLocation.X, CursorLocation.Y, 50.0f));
	}
	// Temporary 
	/*else if (TargetedCharacter != nullptr && !bIsAimingAbility)
	{
		if (TargetedCharacter != this)
		{
			ChangePossession(TargetedCharacter);
		}
	}*/
	else if (bIsAimingAbility && CharacterAbilityComponent != nullptr)
	{
		if (CharacterAbilityComponent->AffectedTiles.Num() > 0)
		{
			CharacterAbilityComponent->ExecuteAbility();
		}
	}
}

void APlayerCharacter::HandleEscape()
{
	if (bIsAimingMovement || bIsAimingAbility)
	{
		if (bIsAimingAbility)
		{
			CharacterAbilityComponent->CancelAbility();
		}
		if (bIsAimingMovement)
		{
			ToogleMovement();
		}
	}
	else
	{
		OnEscapeToogle();
	}
}

void APlayerCharacter::ChangePossession(ABaseCharacter* newCharacter)
{
	CleanupPathfinding(); // PLACEHOLDER
	EndTurn();
	APlayerCharacter* newPlayerCharacter = Cast<APlayerCharacter>(newCharacter);
	newPlayerCharacter->CharacterSpringArm->SetWorldRotation(this->CharacterSpringArm->GetComponentRotation());
	newPlayerCharacter->CharacterSpringArm->SetWorldLocation(newPlayerCharacter->GetActorLocation());
	newPlayerCharacter->DefaultController = this->DefaultController;
	this->DefaultController->UnPossess();
	this->DefaultController->Possess(newPlayerCharacter);
	newPlayerCharacter->BeginTurn();

	//Cleanup this pawn
	this->CharacterSpringArm->SetWorldLocation(this->GetActorLocation());
	this->DefaultController = nullptr;

}

bool APlayerCharacter::IsCursorOnValidMovementTile(int32 x, int32 y)
{
	if (Pathfinder != nullptr && Grid != nullptr)
	{
		return Pathfinder->ValidIndexMap.Contains(x * Grid->Columns + y);
	}
	return false;
	
}

void APlayerCharacter::HandleInventoryToogle()
{
	if (bIsInventoryCollapsed)
	{
		bIsInventoryCollapsed = false;
		OnInventoryOpen();
	}
	else
	{
		bIsInventoryCollapsed = true;
		OnInventoryClose();
	}
}

void APlayerCharacter::EndTurn()
{
	bIsAimingAbility = false;
	bIsAimingMovement = false;
	
	Super::EndTurn();
}

void APlayerCharacter::BeginTurn()
{
	if (Grid && CharacterHealth && CharacterAttributes && Pathfinder && Mover && !bCanAct && !bCanMove && !bIsActive && Indicator && DefaultController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Components are valid at BeginTurn"));
		bCanAct = true;
		bCanMove = true;
		bIsActive = true;
		bAction = true;
		bMovementAction = true;
		bBonusAction = true;
		CurrentSpeed = CharacterAttributes->Stats.Speed / 5.0f;

		OnStatChange();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Components are invalid at BeginTurn"));
	}
}

void APlayerCharacter::BeginPreparationTurn()
{
	if (Grid && CharacterHealth && CharacterAttributes && Pathfinder && Mover && !bCanAct && !bCanMove && !bIsActive && Indicator && DefaultController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Components are valid at BeginTurn"));
		bCanAct = true;
		bCanMove = false;
		bIsActive = true;
		bAction = false;
		bMovementAction = false;
		bBonusAction = false;
		CurrentSpeed = 0;

		OnStatChange();
	}
}

void APlayerCharacter::OnInventoryChange_Implementation(){}
void APlayerCharacter::OnInventoryOpen_Implementation() {}
void APlayerCharacter::OnInventoryClose_Implementation() {}
void APlayerCharacter::OnEscapeToogle_Implementation() {}
void APlayerCharacter::OnVictory_Implementation() {}
void APlayerCharacter::OnDefeat_Implementation() {}
