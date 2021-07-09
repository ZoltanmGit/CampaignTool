// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridField.generated.h"

UENUM(BlueprintType)
enum FieldState
{
	Empty UMETA(DisplayName="Emtpy"),
	Occupied UMETA(DisplayName = "Occupied")
};


UCLASS()
class CAMPAIGNTOOL_API AGridField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FieldAttributes)
		FVector FieldLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FieldAttributes)
		float Row;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FieldAttributes)
		float Column;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FieldAttributes)
		class UStaticMeshComponent* FieldObjectMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FieldAttributes)
		class UBoxComponent* FieldCollisionBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FieldAttributes)
		TEnumAsByte<FieldState> FieldState;
};
