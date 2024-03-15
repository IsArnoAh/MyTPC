#include "GameManager.h"

TArray<FString> GameManager::SaveSlots;  // 存档队列
/**
 * @brief 循环队列式进行存档
 */
void GameManager::SaveGame()
{
	// 同步保存方法
	// if (UGameSaves*TestSave=Cast<UGameSaves>(UGameplayStatics::CreateSaveGameObject(UGameSaves::StaticClass())))
	// {
	// 	// 设置savegame对象上的数据。
	// 	TestSave->SaveIndex = 1;
	//
	// 	// 即时保存游戏。
	// 	if (UGameplayStatics::SaveGameToSlot(TestSave, "Save", TestSave->SaveIndex))
	// 	{
	// 		// 成功保存。
	// 	}
	// 	
	// }
	
	if (UGameSaves* SaveGameInstance = Cast<UGameSaves>(UGameplayStatics::CreateSaveGameObject(UGameSaves::StaticClass())))
	{
		// 设置savegame对象上的数据
		SaveGameInstance->SaveIndex = 1;
		// 生成唯一的存档名称
		const FString SaveSlotName = FString::Printf(TEXT("Saves_%d"), CurrentSaveIndex);
		// 启动异步保存进程。
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveSlotName, SaveGameInstance->SaveIndex, nullptr);
		SaveSlots.Add(SaveSlotName);
		// 如果队列超过最大存档数量，删除队列头部的最早存档
		if (SaveSlots.Num() > MaxSaveSlots)
		{
			const FString EarliestSaveSlot = SaveSlots[0];
			SaveSlots.RemoveAt(0);
			// 在这里执行删除存档的操作，例如使用 UGameplayStatics::DeleteGameInSlot 函数
			UGameplayStatics::DeleteGameInSlot(EarliestSaveSlot, SaveGameInstance->SaveIndex);
		}
		// 更新当前存档索引
		CurrentSaveIndex = (CurrentSaveIndex + 1) % MaxSaveSlots;
	}
}

// 加载游戏
UGameSaves* GameManager::LoadGame(FString slotName, int32 userIndex)
{
	// 同步加载方法
	// 检索并将USaveGame对象投射到UMySaveGame。
	if (UGameSaves* LoadedGame = Cast<UGameSaves>(UGameplayStatics::LoadGameFromSlot(slotName, userIndex)))
	{
		
		return LoadedGame;
	}
	
	return nullptr;
}
