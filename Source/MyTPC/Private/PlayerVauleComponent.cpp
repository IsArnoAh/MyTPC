// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\PlayerValueComponent.h"

// Sets default values for this component's properties
UPlayerValueComponent::UPlayerValueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UPlayerValueComponent::BeginPlay()
{
	Super::BeginPlay();
	//初级参数设置
	Level=1;
	CurrentHealth=100.0f;
	CurrentXP=0;
	MaxHealth=100.0f;
	MaxXP=100.0f;
	// ...
	
}


// Called every frame
void UPlayerValueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//血量增减和死亡判定
float UPlayerValueComponent::IncreaseHealth(float IncreaseHealth)
{
	CurrentHealth+=IncreaseHealth;
	if (CurrentHealth>=MaxHealth)
	{
		CurrentHealth=MaxHealth;
	}
	return CurrentHealth/MaxHealth;
}
float UPlayerValueComponent::DecreaseHealth(float DecreaseHealth)
{
	CurrentHealth-=DecreaseHealth;
	if (CurrentHealth<=0)
	{
		CurrentHealth=0;
	}
	return CurrentHealth/MaxHealth;
}
//经验与等级
void UPlayerValueComponent::IncreaseXP(float IncreaseXP)
{
	CurrentXP+=IncreaseXP;
	if (CurrentXP>=MaxXP)
	{
		CurrentXP-=MaxXP;
		IncreaseLevel();
	}
}
void UPlayerValueComponent::IncreaseLevel()
{
	Level++;
	SkillPoints++;
}




