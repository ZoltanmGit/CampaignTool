// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAbility.generated.h"


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
	AOE UMETA(DisplayName = "AreaOfEffect"),
	Single UMETA(DisplayName = "SingleTarget")
};

UENUM(BlueprintType) // Only for AoE spells
enum EAreaOfEffectType
{
	Line UMETA(DisplayName = "Line"),
	Cone UMETA(DisplayName = "Cone"),
	Sphere UMETA(DisplayName = "Sphere")
};

UENUM(BlueprintType) // Only for single target spells
enum ESingleTargetType
{
	NonAttack UMETA(DisplayName = "NonAttack"),
	MeleeAttack UMETA(DisplayName = "MeleeAttack"),
	RangedAttack UMETA(DisplayName = "RangedAttack")
};

UENUM(BlueprintType)
enum ERequirementType
{
	NoRequirement UMETA(DisplayName = "NoRequiremet"),
	Verbal UMETA(DisplayName = "Verbal"),
	Somantic UMETA(DisplayName = "Somantic"),
	Material UMETA(DisplayName = "Material")
};

UENUM(BlueprintType)
enum EAffectedTargetType
{
	Self UMETA(DisplayName = "Self"),
	Ally UMETA(DisplayName = "Ally"),
	Enemy UMETA(DisplayName = "Enemy"),
	Everyone UMETA(DisplayName = "Everyone")
};

UCLASS()
class CAMPAIGNTOOL_API UBaseAbility : public UObject
{
	GENERATED_BODY()
	
public:
	//Ability Details
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilityDetails)
		FString Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 AbilityLevel;

	//Enum properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilityDetails)
		TEnumAsByte<ECastTime> CastTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilityDetails)
		TEnumAsByte<ETargetType> TargetType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilityDetails)
		TArray<TEnumAsByte<ERequirementType>> AbilityRequirements;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilityDetails)
		TEnumAsByte<EAffectedTargetType> AfffectedTarget;

	//Misc
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Particle)
		class UParticleSystem* AbilityParticle; //We pass this to the AEmmiter / UParticleSystemComponent to the Character's asset which will conttain the spell data

	UFUNCTION(BlueprintCallable)
		virtual void OnSelect() {};
	UFUNCTION(BlueprintCallable)
		virtual void OnCommit() {};
};
