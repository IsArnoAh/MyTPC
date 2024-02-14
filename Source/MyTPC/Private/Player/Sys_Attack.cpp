// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Sys_Attack.h"

#include "MyTPC/MyTPCCharacter.h"

// Sets default values for this component's properties
USys_Attack::USys_Attack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// 攻击动画序列填充
	SwrodAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Attack3_Montage.Attack3_Montage")));
	SwrodAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Attack1_Montage.Attack1_Montage")));
	SwrodAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Attack2_Montage.Attack2_Montage")));
	SwrodAttackMontages.Add(LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Kick_Montage.Kick_Montage")));
	
}


// Called when the game starts
void USys_Attack::BeginPlay()
{
	AttackIndex=0;
	Super::BeginPlay();
	
	// ...
	
}


// Called every frame
void USys_Attack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


int USys_Attack::AttackIndexChange(TArray<UAnimMontage*> tempAnimArray)
{
	AttackIndex++;
	if (AttackIndex>=tempAnimArray.Num())
	{
		AttackIndex=0;
	}
	return AttackIndex;
}

// switch (AttackIndex)
// 	{
// 	case 0:
// 		AttackIndex+=1;return 0;
// 	case 1:
// 		AttackIndex+=1;return 1;
// 	case 2:
// 		AttackIndex+=1;return 2;
// 	case 3:
// 		AttackIndex+=1;return 3;
// 	default:AttackIndex=0;SwordAttack();return 0;
// }



void USys_Attack::ReSetSwordAttackIndex()
{
	AttackIndex=0;
}

bool USys_Attack::GetAttacking()
{
	return bAttacking;
}

void USys_Attack::IsAttacking()
{
	bAttacking=true;
}

UAnimMontage* USys_Attack::TestAttack(WeaponType HoldWeapon, AMyTPCCharacter* Character)
{
	
	UAnimMontage* MontageToPlay;
	switch (HoldWeapon)
	{
	case Punch:
			MontageToPlay = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Kick_Montage.Kick_Montage"));
		break;
	case Sword:
			MontageToPlay = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Attack1_Montage.Attack1_Montage"));
		break;
	default:
			MontageToPlay = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Extra/RPG_Animation/Combat/SwordAttack/Kick_Montage.Kick_Montage"));
		break;
	}

	if (MontageToPlay != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("this is a ScreenDebugMessage"));
	}
	
	return MontageToPlay;
}



