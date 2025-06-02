#include "../utils/helpers.h"
#include "drone.h"

extern uintptr_t baseAddress;
bool lock = false;

void droneJump(bool enabled)
{
    uintptr_t drone1Ptr = GetPointer(baseAddress, offsets::Drone1);
    if (!drone1Ptr || drone1Ptr < 0x10000)
        return;

    CDrone1* drone1 = reinterpret_cast<CDrone1*>(drone1Ptr);

    if (enabled)
        drone1->DroneJump = 2;
    else
        drone1->DroneJump = 1;
}

void droneJumpCooldown(bool enabled)
{
    uintptr_t drone1Ptr = GetPointer(baseAddress, offsets::Drone1);
    if (!drone1Ptr || drone1Ptr < 0x10000)
        return;

    CDrone1* drone1 = reinterpret_cast<CDrone1*>(drone1Ptr);

    if (enabled)
        drone1->DroneJumpDelay = 0.0f;
    else
        drone1->DroneJumpDelay = 2.0f;
}

void droneAmmo(bool enabled)
{
    uintptr_t drone1Ptr = GetPointer(baseAddress, offsets::Drone1);
    uintptr_t drone2Ptr = GetPointer(baseAddress, offsets::Drone2);
    if (!drone1Ptr || drone1Ptr < 0x10000 || !drone2Ptr || drone2Ptr < 0x10000)
        return;

    CDrone1* drone1 = reinterpret_cast<CDrone1*>(drone1Ptr);
    CDrone2* drone2 = reinterpret_cast<CDrone2*>(drone2Ptr);

    if (enabled)
    {
        drone1->DroneMaxAmmo = 2;
        drone2->DroneAmmo = 1;
        lock = false;
    }

    else if (!enabled && !lock)
    {
        drone1->DroneMaxAmmo = 3;
        drone2->DroneAmmo = 0;
        lock = true;
    }
}

void droneRange(bool enabled)
{
    uintptr_t drone1Ptr = GetPointer(baseAddress, offsets::Drone1);
    if (!drone1Ptr || drone1Ptr < 0x10000)
        return;

    CDrone1* drone1 = reinterpret_cast<CDrone1*>(drone1Ptr);

    if (enabled)
        drone1->DroneWeaponReach = 999;
    else
        drone1->DroneWeaponReach = 7;
}

void droneGravity(float gravity)
{
    float originalGravity = -9.8f;

    uintptr_t drone2Ptr = GetPointer(baseAddress, offsets::Drone2);
    if (!drone2Ptr || drone2Ptr < 0x10000)
        return;

    CDrone2* drone2 = reinterpret_cast<CDrone2*>(drone2Ptr);

    drone2->DroneGravity = originalGravity * gravity;
}

void droneSpeed(float speed)
{
    float originalSpeed = 3.5f;
    float originalAccel = 4.0f;

    uintptr_t drone3Ptr = GetPointer(baseAddress, offsets::Drone3);
    if (!drone3Ptr || drone3Ptr < 0x10000)
        return;

    CDrone3* drone3 = reinterpret_cast<CDrone3*>(drone3Ptr);

    drone3->DroneTopSpeed = originalSpeed * speed;
    drone3->DroneAcceleration = originalAccel * speed;
}