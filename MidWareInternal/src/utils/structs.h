#pragma once
#include <Windows.h>

//I CAPITALIZED THE I SO ADJUST OLD CODE ACCORDINGLY
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

	// toggles
	bool infiniteAmmo = false;
	bool rapidFire = false;
	bool noSpread = false;
	bool instaKill = false;
	bool runAndShoot = false;
	bool boltScript = false;
	bool knifeReach = false;
	int recoilReduction = 0;
	int spreadReduction = 0;
	int caliberIndex = 15; // Unknown Caliber
};

struct WeaponSettingsGlobal
{
	bool toggleRunShoot = false;
	bool toggleGlowESP = false;
	bool toggleBoltScript = false;

	bool knifeReach = false;
	bool runShoot = false;
	bool boltScript = false;
	bool glowESP = false;
};