#pragma once

#include "GameSaves.h"
#include "Kismet/GameplayStatics.h"

class GameManager
{
public:
	// 最大存档数量
	static constexpr int32 MaxSaveSlots = 5;
	// 当前存档索引
	inline static int32 CurrentSaveIndex = 0;     
	static  TArray<FString> SaveSlots;
	// 保存游戏
	static void SaveGame();
	// 加载存档
	static UGameSaves* LoadGame(FString slotName,int32 userIndex);
};
