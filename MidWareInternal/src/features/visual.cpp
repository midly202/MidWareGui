#include "../utils/helpers.h"
#include "../utils/structs.h"
#include "visual.h"

extern uintptr_t baseAddress;
extern CheatSettings cheatSettings;

void removeSky()
{
    uintptr_t lightMgrPtr = GetPointer(baseAddress, offsets::LightManager);
    if (!lightMgrPtr || lightMgrPtr < 0x10000)
        return;

    Skybox* skybox = reinterpret_cast<Skybox*>(lightMgrPtr);

    skybox->SkyBox = 0;
}

void restoreSky()
{
    uintptr_t lightMgrPtr = GetPointer(baseAddress, offsets::LightManager);
    if (!lightMgrPtr || lightMgrPtr < 0x10000)
        return;

    Skybox* skybox = reinterpret_cast<Skybox*>(lightMgrPtr);

    skybox->SkyBox = 1;
}

float setPlayerFOV(float playerFOV)
{
    uintptr_t settingsPtr = GetPointer(baseAddress, offsets::Settings);
    if (!settingsPtr || settingsPtr < 0x10000)
        return 1.0f;

    Settings* settings = reinterpret_cast<Settings*>(settingsPtr);

    settings->PlayerFOV = playerFOV;
    return playerFOV;
}

float setGunFOV(float gunFOV)
{
    uintptr_t settingsPtr = GetPointer(baseAddress, offsets::Settings);
    if (!settingsPtr || settingsPtr < 0x10000)
        return 1.0f;

    Settings* settings = reinterpret_cast<Settings*>(settingsPtr);

    settings->GunFOV = gunFOV;
    return gunFOV;
}

void setCamFOV(float offset)
{
    uintptr_t cameraPtr = GetPointer(baseAddress, offsets::Camera);
    if (!cameraPtr || cameraPtr < 0x10000)
        return;

    CCamera* camera = reinterpret_cast<CCamera*>(cameraPtr);

    camera->CameraFOV = offset;
}

void setCamClippingPlane(float offset)
{
    uintptr_t cameraPtr = GetPointer(baseAddress, offsets::Camera);
    if (!cameraPtr || cameraPtr < 0x10000)
        return;

    CCamera* camera = reinterpret_cast<CCamera*>(cameraPtr);

    camera->CameraRenderDistance = offset;
}

void setCamX(float offset)
{
    uintptr_t cameraPtr = GetPointer(baseAddress, offsets::Camera);
    if (!cameraPtr || cameraPtr < 0x10000)
        return;

    CCamera* camera = reinterpret_cast<CCamera*>(cameraPtr);

    camera->CameraX = offset;
}

void setCamY(float offset)
{
    uintptr_t cameraPtr = GetPointer(baseAddress, offsets::Camera);
    if (!cameraPtr || cameraPtr < 0x10000)
        return;

    CCamera* camera = reinterpret_cast<CCamera*>(cameraPtr);

    camera->CameraY = offset;
}

void setCamZ(float offset)
{
    uintptr_t cameraPtr = GetPointer(baseAddress, offsets::Camera);
    if (!cameraPtr || cameraPtr < 0x10000)
        return;

    CCamera* camera = reinterpret_cast<CCamera*>(cameraPtr);

    camera->CameraZ = offset;
}

void setCamPitch(float offset)
{
    uintptr_t cameraPtr = GetPointer(baseAddress, offsets::Camera);
    if (!cameraPtr || cameraPtr < 0x10000)
        return;

    CCamera* camera = reinterpret_cast<CCamera*>(cameraPtr);

    camera->CameraPitch = offset;
}

void setCamRoll(float offset)
{
    uintptr_t cameraPtr = GetPointer(baseAddress, offsets::Camera);
    if (!cameraPtr || cameraPtr < 0x10000)
        return;

    CCamera* camera = reinterpret_cast<CCamera*>(cameraPtr);

    camera->CameraAngle = offset;
}

void setCamYaw(float offset)
{
    uintptr_t cameraPtr = GetPointer(baseAddress, offsets::Camera);
    if (!cameraPtr || cameraPtr < 0x10000)
        return;

    CCamera* camera = reinterpret_cast<CCamera*>(cameraPtr);

    camera->CameraYaw = offset;
}

void camReset()
{
    cheatSettings.camFOV = 0.0f;
    cheatSettings.camDistance = 0.0f;
    cheatSettings.camX = 0.0f;
    cheatSettings.camY = 0.0f;
    cheatSettings.camZ = 0.0f;
    cheatSettings.camPitch = 0.0f;
    cheatSettings.camRoll = 0.0f;
    cheatSettings.camYaw = 0.0f;
}