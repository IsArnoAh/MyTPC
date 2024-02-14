// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemies.h"
#include "MyTPC/MyTPCCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemies::AEnemies()
{
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

void AEnemies::BackAssassin_Implementation(FVector& RefLocation, FRotator& RefRotation)
{
	IEnemiesInterface::BackAssassin_Implementation(RefLocation, RefRotation);
}


void AEnemies::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor->IsA<AMyTPCCharacter>())
	{
		if (AMyTPCCharacter* MyTPC = Cast<AMyTPCCharacter>(OtherActor))
		{
			MyTPC->CanAssassin=true;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("Assassated Ready")));
	}
}

void AEnemies::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor->IsA<AMyTPCCharacter>())
	{
		if (AMyTPCCharacter* MyTPC = Cast<AMyTPCCharacter>(OtherActor))
		{
			MyTPC->CanAssassin=false;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("Leave Assassted Zone")));
	}
}




