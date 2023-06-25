// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTPCCharacter.h"
#include "MotionwarpingComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AMyTPCCharacter

AMyTPCCharacter::AMyTPCCharacter()
{

	// 胶囊体体积设置
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// 旋转值
	TurnRateGamepad = 50.f;
	// 相机旋转参数设置
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// 移动配置
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	//角色移动部分参数配置
	GetCharacterMovement()->JumpZVelocity = DefaultJumpZVelocity;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed =WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	//创建相机与角色组件弹簧臂
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest=true;
	// 创建相机
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	// 获取当前FollowCamera的位置
	FVector FollowCameraPos = FollowCamera->GetComponentLocation();
	// 将FollowCamera向上偏移100个单位
	FollowCameraPos.Z += 55.0f;
	// 更新FollowCamera的位置
	FollowCamera->SetWorldLocation(FollowCameraPos);

	
	// 创建MotionWarpingComponent组件实例
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	//创建数值组件;
	PlayerValueComponent = NewObject<UPlayerValueComponent>(this, TEXT("PlayerValueComponent"));
	
}

// 输入配置
void AMyTPCCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//操作映射
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyTPCCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run",IE_Pressed,this,&AMyTPCCharacter::Run);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyTPCCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyTPCCharacter::MoveRight);

	//映射操作
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AMyTPCCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AMyTPCCharacter::LookUpAtRate);

}

//镜头移动
void AMyTPCCharacter::TurnAtRate(float Rate)
{
	// 控制镜头左右旋转
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMyTPCCharacter::LookUpAtRate(float Rate)
{
	// 设置镜头上下旋转
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

//角色移动函数
void AMyTPCCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// 定位向前向量
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 获取向前向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// 重置位置
		AddMovementInput(Direction, Value);
	}
}
void AMyTPCCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		
		// 定位向右向量
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// 获取向右向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// 重置位置
		AddMovementInput(Direction, Value);
	}
}
//控制跳跃
void AMyTPCCharacter::Jump()
{
	Super::Jump();
	if (bIsRun)
	{
		GetCharacterMovement()->JumpZVelocity = RunJumpZVelocity;
	}
	else
	{
		GetCharacterMovement()->JumpZVelocity = DefaultJumpZVelocity;
		bIsCrouch=false;
		GetCharacterMovement()->MaxWalkSpeed=WalkSpeed;
		GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
		CameraBoom->TargetArmLength = 250.0f;
		Standing=true;
	}
}
//奔跑函数实现
void AMyTPCCharacter::Run()
{
	bIsRun=!bIsRun;
	if (bIsRun)
	{
		GetCharacterMovement()->MaxWalkSpeed=RunSpeed;
		GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
		bIsCrouch=false;
		Standing=true;
		CameraBoom->TargetArmLength = 250.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed=WalkSpeed;
	}
}
//蹲伏函数实现
void AMyTPCCharacter::MyCrouch()
{
	bIsCrouch=!bIsCrouch;
	if (bIsCrouch)
	{
		GetCharacterMovement()->MaxWalkSpeed=CrouchSpeed;
		bIsRun=false;
		Standing=false;
		GetCapsuleComponent()->SetCapsuleHalfHeight(68.0f);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed=WalkSpeed;
		GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
		Standing=true;
	}

}
//Vault检测更新
bool AMyTPCCharacter::UpdateJudgeVault()
{
	bool newJudge=true;
	if (GetMovementComponent()->IsFalling() || GetMovementComponent()->IsFlying())
	{
		newJudge=false;
	}
	return newJudge;
}


//攻击函数
//暗杀函数
void AMyTPCCharacter::BackAssassin(TArray<int32>& Array)
{
	// Array = GetOverlappingActors()
	// for(auto& Element : Array)
	// {
	// 	// 对于每个元素执行相同的操作
	// }
}

void AMyTPCCharacter::Test()
{
	PlayerValueComponent->DecreaseHealth(10.0f);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s: %f"), TEXT("MyFloatValue"), PlayerValueComponent->CurrentHealth));
}

