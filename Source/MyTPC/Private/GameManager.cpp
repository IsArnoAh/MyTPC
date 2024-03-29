#include "GameManager.h"

TArray<FSlotsDetail> GameManager::SlotsDetails;//存档结构队列
//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("work4"));

/**
 * @brief 更新存档队列
 */
void GameManager::UpDateSlotsDetail()
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const FString SaveGamesDirectory = FPaths::ProjectSavedDir() + TEXT("SaveGames");
	TArray<FString> SaveFiles;
	PlatformFile.FindFiles(SaveFiles, *SaveGamesDirectory, TEXT(".sav"));
	if (SaveFiles.Num() <= 0) return;
	for (const FString& GamesDirectory : SaveFiles)
	{
		FDateTime FileCreationTime = PlatformFile.GetTimeStamp(*GamesDirectory);
		FSlotsDetail SlotDetail{
			FPaths::GetBaseFilename(GamesDirectory),
			FileCreationTime.ToString()
		};
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, SlotDetail.SlotName);
		SlotsDetails.Add(SlotDetail);
	}
}

/**
 * @brief 循环队列式进行存档
 */
void GameManager::SaveGame()
{
	// 同步保存方法
	if (UGameSaves* GameSave = Cast<UGameSaves>(UGameplayStatics::CreateSaveGameObject(UGameSaves::StaticClass())))
	{
		// 生成唯一的存档名称
		const FString SaveSlotName = FString::Printf(TEXT("Saves_%d"), CurrentSaveIndex);
		// 即时保存游戏。
		if (UGameplayStatics::SaveGameToSlot(GameSave, SaveSlotName, GameSave->SaveIndex))
		{
			// 数据录入
			if (SlotsDetails.Num() > MaxSaveSlots)
			{
				const auto [SlotName, SlotDate] = SlotsDetails[0];
				SlotsDetails.RemoveAt(0);
				// 在这里执行删除存档的操作，例如使用 UGameplayStatics::DeleteGameInSlot 函数
				UGameplayStatics::DeleteGameInSlot(SlotName, GameSave->SaveIndex);
			}
			// 更新当前存档索引
			CurrentSaveIndex = (CurrentSaveIndex + 1) % MaxSaveSlots;
		}
	}
}

/**
 * @brief 异步存档
 * @param saveMap 保存的地图
 * @param currentLocation 当前位置
 * @param saveDate 保存日期
 * @param saveValue 保存的数值
 */
void GameManager::GamingSaveGame(FName saveMap, const FVector& currentLocation, FString saveDate,
                                 UPlayerValueComponent* saveValue)
{
	if (UGameSaves* SaveGameInstance = Cast<UGameSaves>(
		UGameplayStatics::CreateSaveGameObject(UGameSaves::StaticClass())))
	{
		// 设置SaveGame对象上的数据
		SaveGameInstance->GameMap = saveMap;
		SaveGameInstance->PlayerLocation = currentLocation;
		SaveGameInstance->SaveDate = saveDate;
		SaveGameInstance->PlayerValue = saveValue;
		// 生成唯一的存档名称
		const FString SaveSlotName = FString::Printf(TEXT("Saves_%d"), CurrentSaveIndex);
		// 启动异步保存进程。
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveSlotName, SaveGameInstance->SaveIndex, nullptr);
		// 获取当前本地时间
		const FDateTime CurrentTime = FDateTime::Now();
		FString TimeString = CurrentTime.ToString();
		const FString FormattedTimeString = CurrentTime.ToString(TEXT("%Y-%m-%d %H:%M:%S"));
		const FSlotsDetail TempSlotsDetails{SaveSlotName,FormattedTimeString
		};
		SlotsDetails.Add(TempSlotsDetails);
		// 如果队列超过最大存档数量，删除队列头部的最早存档
		if (SlotsDetails.Num() > MaxSaveSlots)
		{
			const FSlotsDetail EarliestSaveSlot = SlotsDetails[0];
			SlotsDetails.RemoveAt(0);
			// 在这里执行删除存档的操作，例如使用 UGameplayStatics::DeleteGameInSlot 函数
			UGameplayStatics::DeleteGameInSlot(EarliestSaveSlot.SlotName, SaveGameInstance->SaveIndex);
		}
		// 更新当前存档索引
		CurrentSaveIndex = (CurrentSaveIndex + 1) % MaxSaveSlots;
	}
}


/**
 * @brief 选择性加载游戏存档
 * @param slotName 存档名称默认
 * @param userIndex 玩家索引默认为0
 * @return 指定存档
 */
UGameSaves* GameManager::SelectLoadGame(const FString& slotName, int32 userIndex)
{
	// 同步加载方法
	// 检索并将USaveGame对象投射到UMySaveGame。
	if (UGameSaves* LoadedGame = Cast<UGameSaves>(UGameplayStatics::LoadGameFromSlot(slotName, userIndex)))
	{
		return LoadedGame;
	}
	return nullptr;
}

/**
 * @brief 加载最新存档
 * @return 最新存档
 */
UGameSaves* GameManager::LoadLastGame()
{
	const int preIndex = CurrentSaveIndex - 1;
	const FString SaveSlotName = FString::Printf(TEXT("Saves_%d"), preIndex);
	if (UGameSaves* LoadedGame = Cast<UGameSaves>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
	{
		return LoadedGame;
	}
	return nullptr;
}
