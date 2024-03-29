// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "MainMenuBtnGroup.generated.h"


UCLASS()
class MYTPC_API UMainMenuBtnGroup : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 游戏管理类声明
	GameManager* Gm;
	
	TArray<FString> SaveSlots;
	
	TArray<FString> SlotsDate;
	// UI绑定
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UVerticalBox* VerticalBox;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ButtonGroup",meta=(BindWidget))
	UButton* Btn_NewGame;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ButtonGroup",meta=(BindWidget))
	UButton* Btn_LoadGame;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ButtonGroup",meta=(BindWidget))
	UButton* Btn_Setting;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ButtonGroup",meta=(BindWidget))
	UButton* Btn_ExitGame;

	// 新游戏
	UFUNCTION(BlueprintCallable)
	void NewGame();
	// 异步加载地图
	void LoadLevelAsync(FName LevelName) const;
	// 加载游戏方法
	UFUNCTION(BlueprintCallable)
	void LoadGame();
	UMainMenuBtnGroup(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
};
