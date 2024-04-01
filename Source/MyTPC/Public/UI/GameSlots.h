// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "GameSlots.generated.h"

/**
 * 
 */
UCLASS()
class MYTPC_API UGameSlots : public UUserWidget
{
	GENERATED_BODY()

public:
	// 基本控件定义
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UHorizontalBox* Hb_LineDetail;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UButton* Btn_Cover;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UTextBlock* SlotName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UTextBlock* SlotDate;
	GameManager* Gm;
	
	// 构造函数
	UGameSlots(const FObjectInitializer& ObjectInitializer);
	// 预构造函数
	virtual void NativeConstruct() override;
	// 选择存档加载游戏
	UFUNCTION(BlueprintCallable,Category="LoadGame")
	void LoadGame(FString selectSlotName);
	void LoadLevelAsync(FName levelName);
};
