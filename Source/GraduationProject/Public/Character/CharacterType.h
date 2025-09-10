// C135203_GraduationProject
#pragma once

UENUM(BlueprintType)
enum class ECharacterState: uint8
{
	Unequipped,
	EquippedShield,
	EquippedWeapon,
	EquippedWeaponAndShield
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	Unoccupied,
	Attacking,
	Crouching,
	GettingHit,
	ArmingDisArming,
	Dead
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	NoState,
	Patrolling,
	Chasing,
	Attacking,
	Engaged,
	Dead
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	Death1,
	Death2,
	Death3,
	Death4,
	Max
};