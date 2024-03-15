// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Player//PlayerValueComponent.h"
#include "MyTPC/MyTPCCharacter.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "MainMenuBtnGroup.generated.h"


UCLASS()
class MYTPC_API UMainMenuBtnGroup : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite);
	AMyTPCCharacter* MyPlayer;
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

public:
	// 新游戏
	UFUNCTION(BlueprintCallable)
	void NewGame();
	// 异步加载地图
	void LoadLevelAsync(FName LevelName);
	//
	UFUNCTION(BlueprintCallable)
	void LoadGame();
	UMainMenuBtnGroup(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
};
