// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTPCCharacter.h"
#include "MotionWarping.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AMyTPCCharacter

AMyTPCCharacter::AMyTPCCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = DefaultJumpZVelocity;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed =WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest=true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	// 获取当前FollowCamera的位置
	FVector FollowCameraPos = FollowCamera->GetComponentLocation();

	// 将FollowCamera向上偏移100个单位
	FollowCameraPos.Z += 55.0f;

	// 更新FollowCamera的位置
	FollowCamera->SetWorldLocation(FollowCameraPos);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyTPCCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyTPCCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run",IE_Pressed,this,&AMyTPCCharacter::Run);
	// PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&AMyTPCCharacter::Crouch);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyTPCCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyTPCCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AMyTPCCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AMyTPCCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyTPCCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyTPCCharacter::TouchStopped);
}

void AMyTPCCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AMyTPCCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AMyTPCCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMyTPCCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMyTPCCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyTPCCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
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
		// CameraBoom->TargetArmLength = 300.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed=WalkSpeed;
		GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
		// CameraBoom->TargetArmLength = 250.0f;b
		Standing=true;
	}

}



