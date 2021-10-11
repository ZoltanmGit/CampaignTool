#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SpellSystemData.generated.h"


UENUM(BlueprintType)
enum ECastTime
{
	Action UMETA(DisplayName = "Action"),
	BonusAction UMETA(DisplayName = "BonusAction"),
	Reaction UMETA(DisplayName = "Reaction")
};
UENUM(BlueprintType)
enum ETargetType
{
	AOE UMETA(DisplayName = "AreaOfFEffect"),
	Single UMETA(DisplayName = "SingleTarget")
};

UENUM(BlueprintType)
enum EAreaOfEffectType
{
	NoAOE UMETA(DisplayName = "NoAOE"),
	Line UMETA(DisplayName = "Line"),
	Cone UMETA(DisplayName = "Cone"),
	Sphere UMETA(DisplayName = "Sphere")
};
UENUM(BlueprintType)
enum ERequirementType
{
	Verbal UMETA(DisplayName = "Verbal"),
	Somantic UMETA(DisplayName = "Somantic"),
	Material UMETA(DisplayName = "Material")
};
UENUM(BlueprintType)
enum EAffectedTarget
{
	Self UMETA(DisplayName = "Self"),
	Ally UMETA(DisplayName = "Ally"),
	Enemy UMETA(DisplayName = "Enemy"),
	Everyone UMETA(DisplayName = "Everyone")
};

USTRUCT(BlueprintType)
struct FSpellData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 SpellLevel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 SpellValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 DifficultyClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool Concentration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<TEnumAsByte<ERequirementType>> ComponentsArray;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<ECastTime> CastTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<ETargetType> TargetType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<EAreaOfEffectType> AreaOfEffectType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<EAffectedTarget> AffectedTarget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UParticleSystem* SpellParticles;
	
};

