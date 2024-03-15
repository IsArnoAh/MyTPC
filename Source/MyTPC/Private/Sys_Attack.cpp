// Fill out your copyright notice in the Description page of Project Settings.

#include "Sys_Attack.h"
#include "MyTPC/MyTPCCharacter.h"

// Sets default values for this component's properties
USys_Attack::USys_Attack()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USys_Attack::BeginPlay()
{
	// 攻击动画序列填充-剑
	SwrodAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Attack3_Montage.Attack3_Montage")));
	SwrodAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Attack1_Montage.Attack1_Montage")));
	SwrodAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Attack2_Montage.Attack2_Montage")));
	SwrodAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Kick_Montage.Kick_Montage")));

	// 攻击动画序列填充-拳头
	PunchAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/Punching/MmaKick_UE_Montage.MmaKick_UE_Montage")));
	PunchAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/Punching/Punching_L_Montage.Punching_L_Montage")));
	PunchAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/Punching/Punching_R_Montage.Punching_R_Montage")));
	PunchAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/Punching/Punching_kick_Montage.Punching_kick_Montage")));
	
	// 受攻击动画填充
	BeAttackedReact.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/React/HitReact_Right_Montage.HitReact_Right_Montage")));
	BeAttackedReact.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/React/HitReact_Left_Montage.HitReact_Left_Montage")));
	// 攻击索引默认
	AttackIndex=0;
	Super::BeginPlay();
}


// Called every frame
void USys_Attack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/**
 * @brief 
 * @param tempAnimArray 动画容器
 * @return 容器索引
 */
int USys_Attack::AttackIndexChange(TArray<UAnimMontage*> tempAnimArray)
{
	AttackIndex++;
	if (AttackIndex>=tempAnimArray.Num())
	{
		AttackIndex=0;
	}
	return AttackIndex;
}

// 重置攻击参数
void USys_Attack::ReSetAttackIndex()
{
	AttackIndex=0;
}

//受攻击反馈
void USys_Attack::BeAttacked(ACharacter* Character)
{
	// 随机播放攻击动画
	int RandIndex=FMath::RandRange(0,1);
	Character->PlayAnimMontage(BeAttackedReact[RandIndex]);
	
}




