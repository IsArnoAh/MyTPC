#pragma once

UENUM(BlueprintType)
enum CharacterState {
	Idle,
	Walking,
	Running,
	Crouching,
	Jumping,
	Vaulting,
	Talking,
	Shopping,
	Attacking,
	BeAttacked,
	Assassinating,
	Dead
};

UENUM(BlueprintType)
enum WeaponType
{
	Punch,
	Sword,
	Gun
};

UENUM(BlueprintType)
enum AreaStatue
{
	RealWorld,
	DreamWorld,
	BattleField,
	SafeArea,
};

UENUM(BlueprintType)
enum GameStatue
{
	Pause,
	Gaming,
	CG
};

