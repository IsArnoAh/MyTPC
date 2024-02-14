#pragma once

UENUM(BlueprintType)
enum CharacterState {
	Idle,
	Walking,
	Running,
	Crouching,
	Vaulting,
	Talking,
	Shopping,
	Attacking,
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
