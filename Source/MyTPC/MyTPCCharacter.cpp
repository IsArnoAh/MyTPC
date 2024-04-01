// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTPCCharacter.h"
#include "MotionwarpingComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
// #include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AMyTPCCharacter
/**
 * @brief 角色构造函数
 */
AMyTPCCharacter::AMyTPCCharacter()
{
	// 初始枚举状态
	CurrentState = Idle;
	CurrentWeapon = Punch;
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
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//创建相机与角色组件弹簧臂
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = true;

	// 创建相机
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 创建MotionWarpingComponent组件实例
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	//创建数值组件;
	PlayerValue = CreateDefaultSubobject<UPlayerValueComponent>(TEXT("PlayerValue"));
	//创建战斗组件
	Sys_Attack = CreateDefaultSubobject<USys_Attack>(TEXT("SysAttack"));
}

/**
 * @brief 
 * @param PlayerInputComponent 控制器输入组件
 */
void AMyTPCCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//操作映射
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyTPCCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMyTPCCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMyTPCCharacter::StopRunning);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyTPCCharacter::MyCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyTPCCharacter::StopCrouch);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyTPCCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyTPCCharacter::MoveRight);

	//映射操作
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AMyTPCCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AMyTPCCharacter::LookUpAtRate);
}

/**
 * @brief 事件开始时
 */
void AMyTPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(CameraMoveTimerHandle, this, &AMyTPCCharacter::UpdateCameraArmLength, 0.01f, true);

	// 判断存档加载
	LoadValueConfig();
}

/**
 * @brief 镜头移动函数
 * @param Rate 镜头偏移阈值
 */
void AMyTPCCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMyTPCCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

/**
 * @brief 角色移动值
 * @param Value 0-1的轴量
 */
void AMyTPCCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		if (bIsCrouch || CurrentState == Running)
		{
		}
		else
		{
			CurrentState = Walking;
		}
		// 定位向前向量
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// 获取向前向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// 重置位置更新状态
		AddMovementInput(Direction, Value);
	}
}

void AMyTPCCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		if (bIsCrouch || CurrentState == Running)
		{
		}
		else
		{
			CurrentState = Walking;
		}
		// 定位向右向量
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// 获取向右向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// 重置位置
		AddMovementInput(Direction, Value);
	}
}

/**
 * @brief 奔跑函数实现
 */
void AMyTPCCharacter::Run()
{
	if (CurrentState == Idle || CurrentState == Walking)
	{
		const FVector ZeroVector(0.0f, 0.0f, 0.0f);
		const FVector IsVector(RootComponent->GetComponentVelocity());
		CurrentState = Running;
		if (IsVector != ZeroVector)
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
			bIsCrouch = false;
			// Standing=true;
			UpdateArmLengthParameters(250.0f, 0.8f);
		}
		else
		{
			StopRunning();
			CurrentState = Walking;
		}
	}
}

void AMyTPCCharacter::StopRunning()
{
	CurrentState = Walking;
	UpdateArmLengthParameters(170.0f, 1.0f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

/**
 * @brief 控制跳跃函数实现
 */
void AMyTPCCharacter::Jump()
{
	Super::Jump();
	if (CurrentState == Running || CurrentState == Walking)
	{
		if (CurrentState == Running)
		{
			GetCharacterMovement()->JumpZVelocity = RunJumpZVelocity;
			CurrentState = Jumping;
		}
		else
		{
			GetCharacterMovement()->JumpZVelocity = DefaultJumpZVelocity;
			bIsCrouch = false;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
			CameraBoom->TargetArmLength = 170.0f;
			// Standing=true;
		}
	}
}

/**
 * @brief 蹲伏函数实现
 */
void AMyTPCCharacter::MyCrouch()
{
	if (CurrentState == Idle || CurrentState == Walking || CurrentState == Running || CurrentState == Assassinating)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		// 更新FollowCamera的位置
		bIsRun = false;
		// Standing=false;
		bIsCrouch = true;
		GetCapsuleComponent()->SetCapsuleHalfHeight(68.0f);
		UpdateArmLengthParameters(90.0f, 0.8);
		CurrentState = Crouching;
	}
}

void AMyTPCCharacter::StopCrouch()
{
	bIsCrouch = false;
	// 更新FollowCamera的位置
	FollowCamera->SetRelativeLocation(FVector(0, 0, 55));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	UpdateArmLengthParameters(170.0f, 0.8f);
	// Standing=true;
	CurrentState = Idle;
}

/**
 * @brief 翻越参数判断
 * @return 是否可再次翻越
 */
bool AMyTPCCharacter::UpdateJudgeVault()
{
	bool newJudge = true;
	if (GetMovementComponent()->IsFalling() || GetMovementComponent()->IsFlying())
	{
		newJudge = false;
	}
	return newJudge;
}

/**
 * @brief 延迟攻击判定
 */
void AMyTPCCharacter::DelayedSetAttacking(float delayTime)
{
	GetWorld()->GetTimerManager().SetTimer(DelayedAttackHandle, this, &AMyTPCCharacter::SetAttacking, delayTime, false);
}

/**
 * @brief 重置攻击
 */
void AMyTPCCharacter::SetAttacking()
{
	Sys_Attack->bAttacking = false;
}

/**
 * @brief 更改相机参数
 * @param TargetArmLength 相机弹簧臂长度
 * @param LerpSpeed 缩放速度
 */
void AMyTPCCharacter::UpdateArmLengthParameters(float TargetArmLength, float LerpSpeed)
{
	CameraTargetArmLength = TargetArmLength;
	CameraLerpSpeed = LerpSpeed;
	GetWorldTimerManager().SetTimer(CameraMoveTimerHandle, this, &AMyTPCCharacter::UpdateCameraArmLength, 0.01f, true);
}

/**
 * @brief 更新弹簧臂
 */
void AMyTPCCharacter::UpdateCameraArmLength() const
{
	const float CurrentArmLength = CameraBoom->TargetArmLength;
	CameraBoom->TargetArmLength = FMath::FInterpTo(CurrentArmLength, CameraTargetArmLength,
	                                               GetWorld()->GetDeltaSeconds(), CameraLerpSpeed);
}

/**
 * @brief 攻击函数
 * @return 攻击索引
 */
int AMyTPCCharacter::Attack()
{
	if (CurrentState == Idle || CurrentState == Running || CurrentState == Walking)
	{
		TArray<UAnimMontage*> currentAttackAnim;
		// 选择武器动画
		switch (CurrentWeapon)
		{
		case Punch: currentAttackAnim = Sys_Attack->PunchAttackMontages;
			break;
		case Sword: currentAttackAnim = Sys_Attack->SwordAttackMontages;
			break;
		case Gun: currentAttackAnim = Sys_Attack->SwordAttackMontages; // 随意填充，以扩展后续更新攻击动画
			break;
		default: currentAttackAnim = Sys_Attack->PunchAttackMontages; // 随意填充，以扩展后续更新攻击动画
			break;
		}
		if (!Sys_Attack->bAttacking)
		{
			const int tempIndex = Sys_Attack->AttackIndexChange(currentAttackAnim);
			PlayAnimMontage(currentAttackAnim[tempIndex]);
			Sys_Attack->bAttacking = true;
			if (tempIndex == 0)
			{
				DelayedSetAttacking(4.0f);
			}
			else
			{
				DelayedSetAttacking(0.8f);
			}
			if (!bAttackTimerActive)
			{
				GetWorldTimerManager().SetTimer(ResetAttackIndexHandle, this, &AMyTPCCharacter::ReAttackTimer, 6.4f,
				                                false);
				bAttackTimerActive = true;
			}
			return tempIndex;
		}
	}
	return 0;
}

/**
 * @brief 一般测试方法
 */
void AMyTPCCharacter::TestFunction()
{
	const FString MapNameWithPrefix = GetWorld()->GetMapName();
	const FString MapName = MapNameWithPrefix.Mid(MapNameWithPrefix.Find("_") + 3);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, MapName);
	const FDateTime CurrentTime = FDateTime::Now();
	const FString TimeString = CurrentTime.ToString(TEXT("%Y-%m-%d %H:%M:%S"));
	const FVector currentLocation=GetActorLocation();
	Gm.GamingSaveGame(*MapName,currentLocation,TimeString,PlayerValue);
	//FString temStr=TEXT("地图名称:%s,当前时间%s",MapFName.ToString(),TimeString);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Save"));
}

/**
 * @brief 攻击重置计时实现
 */
void AMyTPCCharacter::ReAttackTimer()
{
	bAttackTimerActive = false;
	Sys_Attack->ReSetAttackIndex();
}

/**
 * @brief 加载数值配置
 */
void AMyTPCCharacter::LoadValueConfig()
{
	if (Gm.bSelectSave)
	{
		if (const UGameSaves* LoadGameSaves = Gm.SelectLoadGame(Gm.GetSelectSlotName(), Gm.GetUserIndex()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White,TEXT("be select"));
		}
	}
	else
	{
		if (const UGameSaves* LoadGameSaves = Gm.LoadLastGame())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White,TEXT("last Game"));
		}
	}
}

// get和set方法
/**
 * @brief 玩家所处状态机
 * @return 当前状态
 */
CharacterState AMyTPCCharacter::GetCurrentState()
{
	return CurrentState;
}

/**
 * @brief 设置玩家状态机
 * @param newStatue 新状态机
 */
void AMyTPCCharacter::SetCurrentState(CharacterState newStatue)
{
	CurrentState = newStatue;
}

/**
 * @brief 获取玩家所持武器
 * @return 当前所持武器
 */
WeaponType AMyTPCCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

/**
 * @brief 设置玩家所持武器
 * @param newWeapon 玩家新持武器
 */
void AMyTPCCharacter::SetCurrentWeapon(WeaponType newWeapon)
{
	CurrentWeapon = newWeapon;
}

/**
 * @brief 获取玩家数值
 * @return 玩家数值组件引用
 */
UPlayerValueComponent* AMyTPCCharacter::GetPlayerValue()
{
	return PlayerValue;
}
