// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameSlots.h"

UGameSlots::UGameSlots(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UGameSlots::NativeConstruct()
{
	// if (Btn_Cover)
	// {
	// 	FScriptDelegate LoadGame;
	// 	LoadGame.BindUFunction(this, "LoadGame");
	// 	Btn_Cover->OnClicked.Add(LoadGame);
	// }
	Super::NativeConstruct();
}

/**
 * @brief 
 * @param selectSlotName 选择的存档
 */
void UGameSlots::LoadGame(FString selectSlotName)
{
	Gm->SetLoadSlotDetail(selectSlotName,0);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, SlotName->GetText().ToString());
	if (const UGameSaves* LoadGameSaves = Gm->SelectLoadGame(selectSlotName, 0))
	{
		 const FName LevelName = LoadGameSaves->GameMap; // 加载的关卡名称
		 FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		 	FSimpleDelegateGraphTask::FDelegate::CreateUObject(this, &UGameSlots::LoadLevelAsync, LevelName),
		 	TStatId(),
		 	nullptr,
		 	ENamedThreads::GameThread
		 );
		//UGameplayStatics::OpenLevel(this, LevelName, true, LevelName.ToString());
	}
}

/**
 * @brief 
 * @param leveName 异步加载打开关卡
 */
void UGameSlots::LoadLevelAsync(FName leveName)
{
	UGameplayStatics::OpenLevel(this, leveName, true, leveName.ToString()); // 加载关卡
}
