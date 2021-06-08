// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridField.generated.h"

UENUM(BlueprintType)
enum FieldState
{
	Empty UMETA(DisplayName="Emtpy"),
	Character UMETA(DisplayName="Character Field"),
	Npc UMETA(DisplayName="Npc Field")
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Location)
		FVector FieldLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Location)
		float Row;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Location)
		float Column;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FieldMesh)
		class UStaticMeshComponent* FieldMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FieldObject)
		class UStaticMeshComponent* ObjectMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FieldState)
		TEnumAsByte<FieldState> FieldState;
public:
	//Functionality
	UFUNCTION(BlueprintCallable)
		void ObjectAdded();
};
