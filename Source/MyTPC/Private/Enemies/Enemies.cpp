// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemies.h"
#include "MyTPC/MyTPCCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemies::AEnemies()
{
	// 创建 SkeletalMeshComponent 组件并附加到 RootComponent
	PlayerRef = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	PlayerRef->SetupAttachment(RootComponent);
	PlayerRef->SetRelativeLocation(FVector(-60,25,-90));
	PlayerRef->SetRelativeRotation(FRotator(0,-90,0));
	
 	BackArea=CreateDefaultSubobject<UBoxComponent>(TEXT("Back Area"));
	BackArea->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BackArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BackArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	BackArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	// 将盒体触发器组件附加到角色上
	BackArea->SetupAttachment(RootComponent);
	//基础配置
	Health=100.0f;
	Mental=100.0f;
	Armor=100.0f;
	BackArea->OnComponentBeginOverlap.AddDynamic(this, &AEnemies::OnOverlapBegin);
	BackArea->OnComponentEndOverlap.AddDynamic(this,&AEnemies::AEnemies::OnOverlapEnd);
	
}

// Called when the game starts or when spawned
void AEnemies::BeginPlay()
{
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
	UAnimMontage* BeAssassinated=LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/Assassin/Assassined_Montage.Assassined_Montage"));
	if (BeAssassinated != nullptr)
	{
		PlayAnimMontage(BeAssassinated);
	}
	RefLocation=PlayerRef->GetComponentTransform().GetLocation();
	RefRotation=GetActorRotation();
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
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
}

void AEnemies::BeAttacked_Implementation(WeaponType PlayerWeapon, float damage, int attackIndex)
{
	
	UAnimMontage* BeAttackedAnim;
	switch (attackIndex)
	{
	case 0:BeAttackedAnim=LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/HitReact_Right_Montage.HitReact_Right_Montage"));
		break;
	case 3:	BeAttackedAnim=LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/HitReact_Left_Montage.HitReact_Left_Montage"));
		break;
		default:BeAttackedAnim=LoadObject<UAnimMontage>(nullptr,TEXT("/Game/Extra/RPG_Animation/Combat/HitReact_Right_Montage.HitReact_Right_Montage"));
	}
	if (BeAttackedAnim != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("this is a ScreenDebugMessage"));
		PlayAnimMontage(BeAttackedAnim);
	}
	IEnemiesInterface::BeAttacked_Implementation(PlayerWeapon, damage, attackIndex);
}


// 刺杀区域进入
void AEnemies::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor->IsA<AMyTPCCharacter>())
	{
		if (AMyTPCCharacter* MyTPC = Cast<AMyTPCCharacter>(OtherActor))
		{
			
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("Assassated Ready")));
	}
}

// 刺杀区域退出
void AEnemies::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor->IsA<AMyTPCCharacter>())
	{
		if (AMyTPCCharacter* MyTPC = Cast<AMyTPCCharacter>(OtherActor))
		{
			
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("Leave Assassted Zone")));
	}
}




