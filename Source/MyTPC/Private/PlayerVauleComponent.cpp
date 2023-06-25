// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerVauleComponent.h"

// Sets default values for this component's properties
UPlayerVauleComponent::UPlayerVauleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerVauleComponent::BeginPlay()
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
void UPlayerVauleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerVauleComponent::IncreaseHealth(float IncreaseHealth)
{
	CurrentHealth+=IncreaseHealth;
	if (CurrentHealth==MaxHealth)
	{
		CurrentHealth=MaxHealth;
	}
}

bool UPlayerVauleComponent::DecreaseHealth(float DecreaseHealth)
{
	bool bDead=false;
	CurrentHealth-=DecreaseHealth;
	if (CurrentHealth<=0)
	{
		bDead=true;
	}
	return bDead;
}

void UPlayerVauleComponent::IncreaseStamina(float IncreaseStamina)
{
	CurrentStamina+=IncreaseStamina;
	if (CurrentStamina==MaxStamina)
	{
		CurrentStamina=MaxStamina;
	}
}

bool UPlayerVauleComponent::DecreaseStamina(float DecreaseStamina)
{
	bool bTired=false;
	CurrentStamina-=DecreaseStamina;
	if (CurrentStamina<=0)
	{
		bTired=true;
	}
	return bTired;
}

void UPlayerVauleComponent::IncreaseXP(float IncreaseXP)
{
	CurrentXP+=IncreaseXP;
	if (CurrentXP>=MaxXP)
	{
		CurrentXP-=MaxXP;
		IncreaseLevel();
	}
}

void UPlayerVauleComponent::IncreaseLevel()
{
	Level++;
	SkillPoints++;
}

