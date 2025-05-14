#pragma once
#pragma once
#include <Windows.h>
#include <vector>

namespace offsets
{
	std::vector<uintptr_t> Settings = { 0x7EF0EB8, 0x10 };
	std::vector<uintptr_t> CPlayer = { 0x5E07850, 0x0, 0x28, 0x0, 0xE0, 0xA8, 0x110 };
	std::vector<uintptr_t> Knife = { 0x5E07850, 0x78, 0x0, 0x0, 0xD8 };
	std::vector<uintptr_t> WeaponComponent = { 0x5E32C50, 0x88, 0x0, 0x30, 0x21, 0x70 };
	std::vector<uintptr_t> CCurrentWeapon2 = { 0x5E32C50, 0x88, 0x0, 0x30, 0x21, 0x70, 0x108, 0x0 };
	std::vector<uintptr_t> CCurrentWeapon3 = { 0x5E32C50, 0x88, 0x0, 0x30, 0x21, 0x70, 0x108, 0x0, 0x150, 0x0 };
	std::vector<uintptr_t> CCurrentWeapon4 = { 0x5E32C50, 0x88, 0x0, 0x30, 0x21, 0x70, 0x108, 0x0, 0x148, 0x0 };
	std::vector<uintptr_t> CCurrentWeapon5 = { 0x5E32C50, 0x88, 0x0, 0x30, 0x21, 0x70, 0x108, 0x0, 0x158, 0x0 };
	std::vector<uintptr_t> CCurrentWeapon6 = { 0x5E32C50, 0x88, 0x0, 0x30, 0x21, 0x70, 0x108, 0x0, 0xB0 };
	std::vector<uintptr_t> CCurrentWeaponCaliber = { 0x5E32C50, 0x88, 0x0, 0x30, 0x21, 0x70, 0x108, 0x0, 0x150, 0x0 };
	std::vector<uintptr_t> LightManager = { 0x5E1F4B8, 0x10, 0x408 };
	std::vector<uintptr_t> ESP = { 0x5E22F50, 0x48, 0x0 };
	std::vector<uintptr_t> GodMode = { 0x5DEC248, 0x10, 0x90, 0xD8, 0x8 };
	std::vector<uintptr_t> NoClip = { 0x5B0F7E8, 0x98, 0x10 };
	std::vector<uintptr_t> Camera = { 0x5D7D6F0, 0x300 };
	std::vector<uintptr_t> Drone1 = { 0x5E32C50, 0x88, 0x0, 0x30, 0x49, 0x0, 0xB90 };
	std::vector<uintptr_t> Drone2 = { 0x5E32C50, 0x88, 0x0, 0x30, 0x49, 0x0 };
	std::vector<uintptr_t> Drone3 = { 0x5E32C50, 0x88, 0x0, 0x30, 0x49, 0x0, 0xC68 };
	std::vector<uintptr_t> THuntWave = { 0x6E3FF30 };
}

struct internalFlags
{
	bool glowESP;
	bool runShoot;
	bool boltScript;
	bool infGadget;
	bool goOutside;
	bool exit;
	uintptr_t runShootAddy1;
	uintptr_t runShootAddy2;
};

// Created with ReClass.NET 1.2 by KN4CK3R

class R6GamerProfileManager
{
public:
	char pad_0008[128]; //0x0008
	class Intermediate1* pIntermediate1; //0x0088
	char pad_0090[4016]; //0x0090
}; //Size: 0x1040

class R6SettingsBase
{
public:
	char pad_0000[16]; //0x0000
	class Settings* pSettings; //0x0010
	char pad_0018[4136]; //0x0018
}; //Size: 0x1040

class Settings
{
public:
	char pad_0000[3000]; //0x0000
	float PlayerFOV; //0x0BB8
	float GunFOV; //0x0BBC
	char pad_0BC0[1096]; //0x0BC0
}; //Size: 0x1008

class PlayerCameraManager
{
public:
	char pad_0000[112]; //0x0000
	class WeaponComponent* pIntermediate5; //0x0070
	char pad_0078[1936]; //0x0078
}; //Size: 0x0808

class WeaponComponent
{
public:
	char pad_0000[264]; //0x0000
	class Intermediate6* pIntermediate6; //0x0108
	char pad_0110[16]; //0x0110
	int8_t gunFireMode; //0x0120
	char pad_0121[3]; //0x0121
	int32_t gunAmmo; //0x0124
	char pad_0128[3811]; //0x0128
}; //Size: 0x100B

class CCurrentWeapon2
{
public:
	char pad_0000[240]; //0x0000
	uint64_t gunSound; //0x00F0
	char pad_00F8[72]; //0x00F8
	uint64_t pGunRecoil; //0x0140
	class Intermediate10* pIntermediate10; //0x0148
	class Intermediate8* pIntermediate8; //0x0150
	class Intermediate17* pIntermediate17; //0x0158
	char pad_0160[3784]; //0x0160
}; //Size: 0x1028

class CCurrentWeapon3
{
public:
	char pad_0000[60]; //0x0000
	int32_t GunFireRate; //0x003C
	uint8_t BPS; //0x0040 Bullets Per Second. Lags the game a lot so not implemented.
	char pad_0041[967]; //0x0041
}; //Size: 0x0408

class CCurrentWeapon4
{
public:
	char pad_0000[108]; //0x0000
	int32_t GunDamage; //0x006C
	int32_t GunWallDamage; //0x0070
	char pad_0074[1940]; //0x0074
}; //Size: 0x0808

class CCurrentWeaponCaliber
{
public:
	char pad_0000[76]; //0x0000
	uint32_t GunCaliber; //0x004C
	char pad_0050[60]; //0x0050
}; //Size: 0x008C

class CPlayer
{
public:
	char pad_0000[88]; //0x0000
	int32_t PlayerSpeed; //0x0058
	char pad_005C[4012]; //0x005C
}; //Size: 0x1008

class CCurrentWeapon5
{
public:
	char pad_0000[32]; //0x0000
	float GunSpread; //0x0020
	char pad_0024[996]; //0x0024
}; //Size: 0x0408

class CCurrentWeapon6
{
public:
	wchar_t currWeaponName[16]; //0x0000
	char pad_0020[1024]; //0x0020
}; //Size: 0x0420

class ESP
{
public:
	char pad_0000[276]; //0x0000
	float espOutlineThickness; //0x0114
}; //Size: 0x0118

class Skybox
{
public:
	char pad_0000[672]; //0x0000
	int8_t SkyBox; //0x02A0
	char pad_02A1[167]; //0x02A1
}; //Size: 0x0348

class Knife
{
public:
	char pad_0000[3744]; //0x0000
	float knifeDistancePlayers; //0x0EA0
	float knifeDistanceWalls; //0x0EA4
	char pad_0EA8[408]; //0x0EA8
}; //Size: 0x1040

class Health
{
public:
	char pad_0000[368]; //0x0000
	int32_t Health; //0x0170
	char pad_0174[12]; //0x0174
}; //Size: 0x0180

class CNoClip
{
public:
	char pad_0000[1792]; //0x0000
	float NoClip; //0x0700
	char pad_0704[316]; //0x0704
}; //Size: 0x0840

class CCamera
{
public:
	char pad_0000[3960]; //0x0000
	float N0000286E; //0x0F78
	float N00002894; //0x0F7C
	float N0000286F; //0x0F80
	float CameraFOV; //0x0F84
	float CameraRenderDistance; //0x0F88
	float CameraX; //0x0F8C
	float CameraY; //0x0F90
	float CameraZ; //0x0F94
	float N00002872; //0x0F98
	float CameraPitch; //0x0F9C
	float CameraAngle; //0x0FA0
	float CameraYaw; //0x0FA4
	char pad_0FA8[152]; //0x0FA8
}; //Size: 0x1040

class CDrone1
{
public:
	char pad_0000[5248]; //0x0000
	uint8_t DroneJump; //0x1480
	char pad_1481[139]; //0x1481
	float DroneJumpDelay; //0x150C
	char pad_1510[4]; //0x1510
	uint8_t DroneMaxAmmo; //0x1514
	char pad_1515[3]; //0x1515
	float DroneWeaponReach; //0x1518
	char pad_151C[3948]; //0x151C
}; //Size: 0x2488

class CDrone2
{
public:
	char pad_0000[168]; //0x0000
	float DroneGravity; //0x00A8
	char pad_00AC[5164]; //0x00AC
	uint8_t DroneWeaponUseDelay; //0x14D8
	char pad_14D9[63]; //0x14D9
	uint8_t DroneAmmo; //0x1518
	char pad_1519[39]; //0x1519
}; //Size: 0x1540

class CDrone3
{
public:
	char pad_0000[9024]; //0x0000
	float DroneTopSpeed; //0x2340
	char pad_2344[8]; //0x2344
	float DroneAcceleration; //0x234C
	char pad_2350[496]; //0x2350
}; //Size: 0x2540

class CThuntWave
{
public:
	char pad_0000[1976]; //0x0000
	int32_t CurrentWave; //0x07B8
	char pad_07BC[2180]; //0x07BC
}; //Size: 0x1040