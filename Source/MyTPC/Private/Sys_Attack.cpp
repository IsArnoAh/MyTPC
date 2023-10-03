// Fill out your copyright notice in the Description page of Project Settings.


#include "Sys_Attack.h"

// Sets default values for this component's properties
USys_Attack::USys_Attack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	// ...
}


// Called when the game starts
void USys_Attack::BeginPlay()
{
	SwordAttackIndex=0;
	Super::BeginPlay();
	
	// ...
	
}


// Called every frame
void USys_Attack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int USys_Attack::SwordAttack()
{
	switch (SwordAttackIndex)
	{
	case 0:
		SwordAttackIndex+=1;return 0;
	case 1:
		SwordAttackIndex+=1;return 1;
	case 2:
		SwordAttackIndex+=1;return 2;
	case 3:
		SwordAttackIndex+=1;return 3;
	default:SwordAttackIndex=0;SwordAttack();return 0;
	}
}

void USys_Attack::ReSetSwordAttackIndex()
{
	SwordAttackIndex=0;
}

