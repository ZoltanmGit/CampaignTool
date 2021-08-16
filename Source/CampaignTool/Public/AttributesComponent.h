// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeEnums.h"
#include "AttributesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMPAIGNTOOL_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	//General
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		FString CharacterName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		int32 level;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		int32 proficiencyBonus;
	
	//Character Attributes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		int32 Strength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		int32 Dexterity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		int32 Intelligence;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		int32 Constitution;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		int32 Wisdom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		int32 Charisma;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		int32 Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		TEnumAsByte<Race> Race;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		TEnumAsByte<Class> Class;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		TEnumAsByte<Alignment> Alignment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		TArray<TEnumAsByte<Faction>> ReputationArray;
	//Collections
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		TArray<TEnumAsByte<Proficiency>> ProficiencyArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
		TArray<TEnumAsByte<Language>> LanguageArray;
	//Character Race and Background
	
	//Skills - Strength
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Athletics;
	//Skills - Dexterity
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Acrobatics;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 SleightOfHand;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Stealth;
	//Skills - Intelligence
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Arcana;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 History;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Investigation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Nature;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Religion;
	//Skills - Wisdom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 AnimalHandling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Insight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Medicine;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Perception;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Survival;
	//Skills - Charisma
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Deception;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Intimidation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Performance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	int32 Persuasion;

public:
	UFUNCTION(BlueprintCallable)
		int32 GetModifier(int32 value);
	UFUNCTION(BlueprintCallable)
		int32 GetStrength();
	UFUNCTION(BlueprintCallable)
		int32 GetDexterity();
	UFUNCTION(BlueprintCallable)
		int32 GetIntelligence();
	UFUNCTION(BlueprintCallable)
		int32 GetWisdom();
	UFUNCTION(BlueprintCallable)
		int32 GetConstitution();
	UFUNCTION(BlueprintCallable)
		int32 GetCharisma();

	UFUNCTION(BlueprintCallable)
		int32 GetAthletics();

	UFUNCTION(BlueprintCallable)
		int32 GetAcrobatics();
	UFUNCTION(BlueprintCallable)
		int32 GetSleightOfHand();
	UFUNCTION(BlueprintCallable)
		int32 GetStealth();

	UFUNCTION(BlueprintCallable)
		int32 GetArcana();
	UFUNCTION(BlueprintCallable)
		int32 GetHistory();
	UFUNCTION(BlueprintCallable)
		int32 GetInvestigation();
	UFUNCTION(BlueprintCallable)
		int32 GetNature();
	UFUNCTION(BlueprintCallable)
		int32 GetReligion();

	UFUNCTION(BlueprintCallable)
		int32 GetAnimalHandling();
	UFUNCTION(BlueprintCallable)
		int32 GetInsight();
	UFUNCTION(BlueprintCallable)
		int32 GetMedicine();
	UFUNCTION(BlueprintCallable)
		int32 GetPerception();
	UFUNCTION(BlueprintCallable)
		int32 GetSurvival();

	UFUNCTION(BlueprintCallable)
		int32 GetDeception();
	UFUNCTION(BlueprintCallable)
		int32 GetIntimidation();
	UFUNCTION(BlueprintCallable)
		int32 GetPerformance();
	UFUNCTION(BlueprintCallable)
		int32 GetPersuasion();
};
