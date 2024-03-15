// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuBtnGroup.h"

// 构造函数
UMainMenuBtnGroup::UMainMenuBtnGroup(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}
// 组件生成构造函数
void UMainMenuBtnGroup::NativeConstruct()
{
	
	Super::NativeConstruct();
}

/**
 * @brief 开始新游戏
 */
void UMainMenuBtnGroup::NewGame()
{
	FName LevelName = FName("Map_Tutorial"); // 替换为你想要加载的关卡名称
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateUObject(this, &UMainMenuBtnGroup::LoadLevelAsync, LevelName),
		TStatId(),
		nullptr,
		ENamedThreads::GameThread
	);
}

/**
 * @brief 
 * @param LevelName 异步加载打开关卡
 */
void UMainMenuBtnGroup::LoadLevelAsync(FName LevelName)
{
	UGameplayStatics::OpenLevel(this, LevelName, true, FString("Map_Tutorial")); // 加载关卡
}

/**
 * @brief 加载游戏
 */
void UMainMenuBtnGroup::LoadGame()
{
	
}
