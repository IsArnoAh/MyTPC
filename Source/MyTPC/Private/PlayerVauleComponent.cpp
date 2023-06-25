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
	CurrentStamina=100.0f;
	CurrentXP=0;
	MaxHealth=100.0f;
	MaxStamina=100.0f;
	MaxXP=100.0f;
	// ...
	
}


// Called every frame
void UPlayerValueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerValueComponent::IncreaseHealth(float IncreaseHealth)
{
	CurrentHealth+=IncreaseHealth;
	if (CurrentHealth==MaxHealth)
	{
		CurrentHealth=MaxHealth;
	}
}

bool UPlayerValueComponent::DecreaseHealth(float DecreaseHealth)
{
	bool bDead=false;
	CurrentHealth-=DecreaseHealth;
	if (CurrentHealth<=0)
	{
		bDead=true;
	}
	return bDead;
}

void UPlayerValueComponent::IncreaseStamina(float IncreaseStamina)
{
	CurrentStamina+=IncreaseStamina;
	if (CurrentStamina==MaxStamina)
	{
		CurrentStamina=MaxStamina;
	}
}

bool UPlayerValueComponent::DecreaseStamina(float DecreaseStamina)
{
	bool bTired=false;
	CurrentStamina-=DecreaseStamina;
	if (CurrentStamina<=0)
	{
		bTired=true;
	}
	return bTired;
}

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

