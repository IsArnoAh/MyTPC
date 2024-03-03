// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemiesInterface.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Enemies.generated.h"

UCLASS()
class MYTPC_API AEnemies : public ACharacter,public IEnemiesInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemies();
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Enemies Config")
	float Health;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Enemies Config")
	float Mental;UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Enemies Config")
	float Armor;
	// 背部暗杀区域
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BackArea;
	// 重定玩家骨骼点
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Component")
	USkeletalMeshComponent* PlayerRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//背部触发器判断
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
					  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable,Category="Attacked")
	//背部刺杀函数实现
	virtual void BackAssassin_Implementation(FVector& RefLocation, FRotator& RefRotation) override;

	virtual void BeAttacked_Implementation(WeaponType PlayerWeapon, float damage, int attackIndex) override;

	UFUNCTION(BlueprintCallable,Category="Attacked")
	void EnterDeath();

private:
	// 延迟死亡计时器
	FTimerHandle DelayedDeathHandle;
};
