// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "FSlotsDetail.h"
#include "GameManager.h"
#include "Blueprint/UserWidget.h"
#include "LoadSlotsPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYTPC_API ULoadSlotsPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	ULoadSlotsPanel(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintReadOnly)
	TArray<FSlotsDetail> SlotsDetails;
	GameManager* Gm;

	UFUNCTION(BlueprintCallable,Category="Test")
	void UpdateLoadSlots();
};
