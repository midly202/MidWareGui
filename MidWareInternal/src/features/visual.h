#pragma once
#include <string>
#include <Windows.h>

void removeSky();

void restoreSky();

float setPlayerFOV(float playerFOV);

float setGunFOV(float gunFOV);

void setCamFOV(float offset);

void setCamClippingPlane(float offset);

void setCamX(float offset);

void setCamY(float offset);

void setCamZ(float offset);

void setCamPitch(float offset);

void setCamRoll(float offset);

void setCamYaw(float offset);

void camReset();