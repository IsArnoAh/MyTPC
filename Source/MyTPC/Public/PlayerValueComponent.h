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
	float CurrentHealth;
	float CurrentXP;
	float MaxXP;
	int Level;
	int SkillPoints;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//方法定义
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
};
