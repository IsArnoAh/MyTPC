// Copyright Epic Games, Inc. All Rights Reserved.


#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionwarpingComponent.h"
#include "PlayerValueComponent.h"
#include "Sys_Attack.h"
#include "MyTPCCharacter.generated.h"


//C:\Program Files\Epic Games\UE_5.0\Engine\Plugins\Experimental\Animation\MotionWarping
UCLASS(config=Game)
class AMyTPCCharacter : public ACharacter
{
	GENERATED_BODY()
	
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	
public:
	AMyTPCCharacter();
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;
	

	//判定参数
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bJudgeVault;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool CanAssassin;
	
	//奔跑状态，蹲伏状态
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bIsRun;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bIsCrouch;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool Standing=true;
	//默认参数设置，跳跃高度，和移动速度，
	UPROPERTY(EditAnywhere, Category = "Jump")
	float DefaultJumpZVelocity = 200.f;
	UPROPERTY(EditAnywhere, Category = "Jump")
	float RunJumpZVelocity = 400.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed=220.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed=600.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float CrouchSpeed=200.0f;


	
	

protected:
	//存放蓝图控件成功案例
	 // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	 // TSubclassOf<UUserWidget> HUDWidgetClass;
	
	virtual void BeginPlay() override;
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);
	//奔跑函数声明
	UFUNCTION(BlueprintCallable,Category="Movement")
	void Run();
	//蹲伏函数声明
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MyCrouch();
	//重写跳函数
	virtual  void Jump() override;
	
	//角色判断参数函数
	UFUNCTION(BlueprintCallable, Category = "Judge")
	bool UpdateJudgeVault();

	UFUNCTION(BlueprintCallable, Category="Attack")
	void BackAssassin(TArray<int32>& Array);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 创建Motion Warping Component
	UPROPERTY(BlueprintReadWrite,Category="Animation")
	UMotionWarpingComponent* MotionWarpingComponent;
	//角色组件
	UPROPERTY(BlueprintReadWrite,Category="Value")
	UPlayerValueComponent* PlayerValueComponent;
	UPROPERTY(BlueprintReadWrite,Category="Attack")
	USys_Attack* Sys_Attack;
	

public:
	/** Returns CameraBoom subject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	
};

