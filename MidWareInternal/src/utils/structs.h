#pragma once
#include <Windows.h>

struct InternalFlags
{
	bool glowESP;
	bool runShoot;
	bool boltScript;
	bool infGadget;
	bool goOutside;
	bool exit;
	uintptr_t runShootAddy1;
	uintptr_t runShootAddy2;
	char weaponName[64];
};

struct WeaponSettings
{
	// original values
	int32_t originalAmmoValue = -1;
	int64_t originalRecoilValue = -1;
	int8_t originalFiremodeValue = -1;
	int32_t originalFirerateValue = -1;
	int32_t originalDamageValue = -1;
	float originalSpreadValue = -1.0f;

	// toggles
	bool infiniteAmmo = false;
	bool noRecoil = false;
	bool fullAuto = false;
	bool rapidFire = false;
	bool instaKill = false;
	bool runAndShoot = false;
	bool boltScript = false;
	bool knifeReach = false;
	int spreadReduction = 0;
	int caliberIndex = 15; // Unknown Caliber
};

struct CheatSettings
{
	bool toggleRunShoot = false;
	bool toggleGlowESP = false;
	bool toggleBoltScript = false;
	bool toggleGoOutside = false;
	bool toggleLockWave = false;
	bool toggleInfGadgets = false;

	bool knifeReach = false;
	bool runShoot = false;
	bool boltScript = false;
	bool godMode = false;
	bool noClip = false;
	bool glowESP = false;
	bool removeSky = false;
	float playerSpeed = 1.0f;
	float playerFOV = 1.0f;
	float gunFOV = 1.0f;
	float camFOV = 0.0f;
	float camDistance = 0.0f;
	float camX = 0.0f;
	float camY = 0.0f;
	float camZ = 0.0f;
	float camPitch = 0.0f;
	float camRoll = 0.0f;
	float camYaw = 0.0f;
};