// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sys_Attack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTPC_API USys_Attack : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USys_Attack();
	bool bCanAttack=true;
	int SwordAttackIndex=0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UFUNCTION(BlueprintCallable,Category="Attack_Sword")
	int SwordAttack();
	UFUNCTION(BlueprintCallable,Category="Attack_Sword")
	void ReSetSwordAttackIndex();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
