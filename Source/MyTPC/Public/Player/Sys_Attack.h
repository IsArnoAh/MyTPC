// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerEnum.h"
#include "Sys_Attack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTPC_API USys_Attack : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USys_Attack();
	// 定义动画序列表
	TArray<UAnimMontage*> SwrodAttackMontages;
	TArray<UAnimMontage*> PunchAttackMontages;
	TArray<UAnimMontage*> EnemiesAttackMontages;
	TArray<UAnimMontage*> BeAttackedReact;
	
	
	bool bCanAttack=true;
	bool bAttacking=false;
	int AttackIndex=0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UFUNCTION(BlueprintCallable,Category="Attack_Sword")
	UAnimMontage* TestAttack(WeaponType HoldWeapon,AMyTPCCharacter* Character);

	UFUNCTION(BlueprintCallable)
	int AttackIndexChange(TArray<UAnimMontage*> tempAnimArray);
	
	UFUNCTION(BlueprintCallable,Category="Attack_Sword")
	void ReSetAttackIndex();
	
	bool GetAttacking();
	
	void IsAttacking();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
