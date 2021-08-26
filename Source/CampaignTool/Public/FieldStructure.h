#pragma once

#include "CoreMinimal.h"
#include "FieldStructure.generated.h"

UENUM(BlueprintType)
enum TerrainType
{
	Void  UMETA(DisplayName = "Void"),  //-1
	Grass UMETA(DisplayName = "Grass"), //0
	Water UMETA(DisplayName = "Water"), //1
	DeepWater UMETA(DisplayName = "DeepWater"), //2
	Wood  UMETA(DisplayName = "Wood"),  //3
	Dirt  UMETA(DisplayName = "Dirt"),  //4
	Rock  UMETA(DisplayName = "Rock")   //5


};
UENUM(BlueprintType)
enum LightType
{
	Bright UMETA(DisplayName = "Bright"),
	Dim UMETA(DisplayName = "Dim"),
	Darkness  UMETA(DisplayName = "Darkness"),
};

USTRUCT(BlueprintType)
struct FTileProperties
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere)
		bool bIsObscured;
	UPROPERTY(VisibleAnywhere)
		bool bIsOccupied;
	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<TerrainType> TerrainType;
	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<LightType> LightType;
	UPROPERTY(VisibleAnywhere)
		int32 Row;
	UPROPERTY(VisibleAnywhere)
		int32 Column;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* ActorOnTile;
};