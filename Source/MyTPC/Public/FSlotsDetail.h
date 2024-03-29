#pragma once

#include "FSlotsDetail.generated.h"
USTRUCT(BlueprintType)
struct FSlotsDetail
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly,DisplayName="SlotName")
	FString SlotName;
	UPROPERTY(BlueprintReadOnly,DisplayName="SlotDate")
	FString SlotDate;
};
