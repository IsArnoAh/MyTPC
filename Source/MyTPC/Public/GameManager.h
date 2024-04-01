#pragma once

#include "GameSaves.h"
#include "FSlotsDetail.h"
#include "Kismet/GameplayStatics.h"

class GameManager
{

private:
	inline static FString SelectSlotName=TEXT("Save_0");
	inline static int32 UserIndex=0;
public:
	// 选择存档参数
	inline static bool bSelectSave=false;

	
	// 最大存档数量
	static constexpr int32 MaxSaveSlots = 10;
	// 当前存档索引
	inline static int32 CurrentSaveIndex = 0;
	// 存档名称数组
	static TArray<FSlotsDetail> SlotsDetails;
	// 保存游戏
	static void SaveGame();
	static void GamingSaveGame(FName saveMap, const FVector& currentLocation,FString saveDate,UPlayerValueComponent* saveValue);
	// 加载存档
	static UGameSaves* SelectLoadGame(const FString& slotName,int32 userIndex);
	// 加载最新存档
	static UGameSaves* LoadLastGame();
	static TArray<FSlotsDetail> UpDateSlotsDetail();
	// 设置加载存档指定参数(get和set方法)
	static void SetLoadSlotDetail(FString selectSlotName,int32 userIndex);
	static FString GetSelectSlotName();
	static int32 GetUserIndex();
};
