// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GamingMenuBtnGroup.h"

#include "MyTPC/MyTPCCharacter.h"
// 构造函数
void UGamingMenuBtnGroup::NativeConstruct()
{
	Super::NativeConstruct();
}

// 组件生成构造函数
UGamingMenuBtnGroup::UGamingMenuBtnGroup(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

/**
 * @brief 返回游戏
 */
void UGamingMenuBtnGroup::BackGame()
{
 CoverPanel->SetVisibility(ESlateVisibility::Hidden);
 
}

/**
 * @brief 保存游戏
 */
void UGamingMenuBtnGroup::SaveGame()
{
 if (AMyTPCCharacter* player= Cast<AMyTPCCharacter>(GetOwningPlayer()))
 {
  // 获取当前系统时间戳
  const double CurrentTimestamp = FPlatformTime::Seconds();

  // 将时间戳转换为 FDateTime 实例
  const FDateTime CurrentTime = FDateTime::FromUnixTimestamp(CurrentTimestamp);

  // 获取小时和分钟
  const int32 Hour = CurrentTime.GetHour();
  const int32 Minute = CurrentTime.GetMinute();

  // 将小时和分钟转换为字符串
  const FString HourString = FString::Printf(TEXT("%02d"), Hour);
  const FString MinuteString = FString::Printf(TEXT("%02d"), Minute);

  // 构建时间字符串（格式：HH:MM）
  const FString TimeString = HourString + TEXT(":") + MinuteString;
  const FName currentMap(GetWorld()->GetMapName());
  Gm->GamingSaveGame(currentMap,player->GetActorLocation(),TimeString,player->GetPlayerValue());
 }
	
}

/**
 * @brief 回到标题页
 */
void UGamingMenuBtnGroup::ReTurnTitle()
{
 UGameplayStatics::OpenLevel(GetWorld(), TEXT("Map_UI"));
}

/**
 * @brief 退出游戏
 */
void UGamingMenuBtnGroup::ExitGame()
{
 
 UGameplayStatics::GetPlayerController(GWorld, 0)->ConsoleCommand("quit");
}
