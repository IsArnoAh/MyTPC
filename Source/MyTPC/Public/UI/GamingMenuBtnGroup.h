// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "GamingMenuBtnGroup.generated.h"

/**
 * 
 */
UCLASS()
class MYTPC_API UGamingMenuBtnGroup : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UVerticalBox* VerticalBox;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ButtonGroup",meta=(BindWidget))
	UButton* Btn_BackGame;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ButtonGroup",meta=(BindWidget))
	UButton* Btn_SaveGame;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ButtonGroup",meta=(BindWidget))
	UButton* Btn_ReTurnTitle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ButtonGroup",meta=(BindWidget))
	UButton* Btn_ExitGame;

public:
	virtual void NativeConstruct() override;
	UGamingMenuBtnGroup(const FObjectInitializer& ObjectInitializer);
};
