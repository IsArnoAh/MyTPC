// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemies.h"
// #include "MyTPC/MyTPCCharacter.h"
#include "Sys_Attack.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyTPC/MyTPCCharacter.h"

// Sets default values
AEnemies::AEnemies()
{
	//创建战斗组件
	Sys_Attack = CreateDefaultSubobject<USys_Attack>(TEXT("Enemies_Combat"));

	// 创建 SkeletalMeshComponent 组件并附加到 RootComponent
	PlayerRef = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	PlayerRef->SetupAttachment(RootComponent);
	PlayerRef->SetRelativeLocation(FVector(-60, 25, -90));
	PlayerRef->SetRelativeRotation(FRotator(0, -90, 0));

	BackArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Back Area"));
	BackArea->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BackArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BackArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	BackArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	// 将盒体触发器组件附加到角色上
	BackArea->SetupAttachment(RootComponent);

	//基础配置
	Health = 100.0f;
	Level = FMath::RandRange(1, 10);
	Damage = Level * 0.5;
	Health = Health * 0.5;
}

// Called when the game starts or when spawned
void AEnemies::BeginPlay()
{
	BackArea->OnComponentBeginOverlap.AddDynamic(this, &AEnemies::OnOverlapBegin);
	BackArea->OnComponentEndOverlap.AddDynamic(this, &AEnemies::OnOverlapEnd);
	Super::BeginPlay();
}


// Called to bind functionality to input
void AEnemies::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 背部刺杀函数实现
void AEnemies::BackAssassin_Implementation(FVector& RefLocation, FRotator& RefRotation)
{
	UAnimMontage* BeAssassinated = LoadObject<UAnimMontage>(
		nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/Assassin/Assassined_Montage.Assassined_Montage"));
	if (BeAssassinated != nullptr)
	{
		PlayAnimMontage(BeAssassinated);
	}
	RefLocation = PlayerRef->GetComponentTransform().GetLocation();
	RefRotation = GetActorRotation();
	IEnemiesInterface::BackAssassin_Implementation(RefLocation, RefRotation);
	GetWorldTimerManager().SetTimer(DelayedDeathHandle, this, &AEnemies::EnterDeath, 2.0f, false);
}

// 角色进入死亡
void AEnemies::EnterDeath()
{
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BackArea->DestroyComponent();
	PlayerRef->DestroyComponent();
	GetCapsuleComponent()->DestroyComponent();
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	bIsDead = true;
}

// // 角色被攻击
// void AEnemies::BeAttacked_Implementation(WeaponType playerWeapon, float damage, int attackIndex)
// {
// 	UAnimMontage* BeAttackedAnim;
// 	switch (attackIndex)
// 	{
// 	case 0:BeAttackedAnim=LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/HitReact_Right_Montage.HitReact_Right_Montage"));
// 		break;
// 	case 3:	BeAttackedAnim=LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/HitReact_Left_Montage.HitReact_Left_Montage"));
// 		break;
// 		default:BeAttackedAnim=LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/HitReact_Right_Montage.HitReact_Right_Montage"));
// 	}
// 	if (BeAttackedAnim != nullptr)
// 	{
// 		Health-=damage;
// 		PlayAnimMontage(BeAttackedAnim);
// 		if (Health<=0)
// 		{
// 			EnterDeath();
// 		}
// 	}
// 	IEnemiesInterface::BeAttacked_Implementation(playerWeapon, damage, attackIndex);
// }

// void AEnemies::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	AMyTPCCharacter* Player = Cast<AMyTPCCharacter>(OtherActor);
// 	if (Player != nullptr)
// 	{
// 		Player->bAssassinReady = true;
// 	}
// 	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Black,TEXT("out"));
// }

// void AEnemies::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	// AMyTPCCharacter* Player = Cast<AMyTPCCharacter>(OtherActor);
// 	// if (Player != nullptr)
// 	// {
// 	// 	Player->bAssassinReady = false;
// 	// }
// 	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Black,TEXT("comng"));
// }

// 背部组件重叠事件
void AEnemies::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyTPCCharacter* Player = Cast<AMyTPCCharacter>(OtherActor);
	if (Player != nullptr)
	{
		Player->bAssassinReady = true;
	}
}

void AEnemies::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex)
{
	AMyTPCCharacter* Player = Cast<AMyTPCCharacter>(OtherActor);
	if (Player != nullptr)
	{
		Player->bAssassinReady = false;
	}
}

// 新攻击反馈
void AEnemies::BeAttacked(WeaponType holdWeapon, float takeDamage)
{
	if (!bIsDead)
	{
		Health -= takeDamage;
		BackArea->DestroyComponent();
		Sys_Attack->BeAttacked(this);
		if (Health <= 0)
		{
			EnterDeath();
		}
	}
}

// 判断死亡
bool AEnemies::IsDead()
{
	return bIsDead;
}
