// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerValueComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTPC_API UPlayerValueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	UPlayerValueComponent();
	//数值定义
	//参数配置
	float MaxHealth;
	float MaxXP;
	float MaxMental;
	float CurrentHealth;
	float CurrentXP;
	float CurrentMental;
	float CurrentDamage;
	int CurrentLevel;
	int MaxLevel;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//方法定义
	UFUNCTION(BlueprintCallable,Category="ValueConfig")
	void FirstGameValueConfig();
	UFUNCTION(BlueprintCallable,Category="ValueConfig")
	void LoadGameValueConfig(float currentHealth,float currentXP,float currentMental,float currentDamage,int currentLevel);

	UFUNCTION(BlueprintCallable,Category="ValueCOnfig")
	void IncreaseMaxHealth();
	UFUNCTION(BlueprintCallable,Category="ValueCOnfig")
	void IncreaseDamage(float increaseValue);

	//血量
	UFUNCTION(BlueprintCallable,Category="Health")
	float IncreaseHealth(float IncreaseHealth);
	UFUNCTION(BlueprintCallable,Category="Health")
	float DecreaseHealth(float DecreaseHealth);
	
	//经验与等级
	UFUNCTION(BlueprintCallable,Category="XP")
	void IncreaseXP(float IncreaseXP);
	UFUNCTION(BlueprintCallable,Category="XP")
	void IncreaseLevel();

	//精神值
	UFUNCTION(BlueprintCallable,Category="Mental")
	float IncreaseMental(float IncreaseMental);
	UFUNCTION(BlueprintCallable,Category="Mental")
	float DecreaseMental(float DecreaseMental);

	UFUNCTION(BlueprintCallable,Category="Damage")
	float GetCurrentDamage();
};
