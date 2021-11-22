#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemStructures.generated.h"

USTRUCT(BlueprintType)
struct FItemProperties : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemProp)
		FString ItemCode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemProp)
		FString Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemProp)
		FString Description;
};