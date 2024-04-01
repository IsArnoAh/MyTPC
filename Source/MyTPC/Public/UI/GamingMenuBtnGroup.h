// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameManager.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
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
	GameManager* Gm;
	TArray<FString> SlotsName;
	// UI绑定
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UCanvasPanel* CoverPanel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UImage* CoverImg;
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


	// 基本函数配置
	// 预构造函数
	virtual void NativeConstruct() override;
	// 构造函数
	UGamingMenuBtnGroup(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void BackGame();
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void ReTurnTitle();
	UFUNCTION(BlueprintCallable)
	void ExitGame();
};
