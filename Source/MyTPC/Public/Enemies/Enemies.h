// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemiesInterface.h"
#include "Sys_Attack.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Enemies.generated.h"

UCLASS()
class MYTPC_API AEnemies : public ACharacter,public IEnemiesInterface
{
	GENERATED_BODY()
	float Health;
	int Level;
	float Damage;
	bool bIsDead;

public:
	// Sets default values for this character's properties
	AEnemies();

	// 背部暗杀区域
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BackArea;
	// 重定玩家骨骼点
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Component")
	USkeletalMeshComponent* PlayerRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite,Category="Enemies_Combat")
	USys_Attack* Sys_Attack; 

public:
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable,Category="Attacked")
	//背部刺杀函数
	virtual void BackAssassin_Implementation(FVector& RefLocation, FRotator& RefRotation) override;
	//受攻击函数
	UFUNCTION(BlueprintCallable,Category="Attacked")
	void EnterDeath();

	UFUNCTION(BlueprintCallable,Category="Attacked")
	void BeAttacked(WeaponType HoldWeapon,float Damage);

	UFUNCTION(BlueprintCallable)
	bool IsDead();

private:
	// 延迟死亡计时器
	FTimerHandle DelayedDeathHandle;
};
