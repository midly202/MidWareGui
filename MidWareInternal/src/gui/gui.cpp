#include "../utils/helpers.h"
#include "gui.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <string>

void InitGUI(HWND window, ID3D11Device* device, ID3D11DeviceContext* context)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(device, context);
}

void BeginGUIFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void RenderGUI()
{
    // Cheat GUI
    extern std::wstring weaponName;

    static int selectedCaliber = 0;
    const char* caliberTypes[] = {
        "Regular Rifle",
        "Regular SMG",
        "Regular Pistol",
        "Tachanka's DP27",
        "Tachanka's DP27 (Turret)",
        "Buck's Skeleton Key",
        "Kali's Sniper",
        "Maverick's Blowtorch",
        "Maestro's Turret",
        "BOSG",
        "Bailiff",
        " "
    };

    static bool godMode = false;
    static bool playerSpeed = false;
    static bool noClip = false;
    static bool infiniteGadgets = false;

    static bool glowESP = false;
    static bool fisheyeFOV = false;
    static bool removeSky = false;
    static bool thirdPerson = false;

    static bool droneNoGravity = false;
    static bool droneSpeed = false;
    static bool canJump = false;
    static bool droneRange = false;
    static bool droneRapidFire = false;
    static bool droneAmmo = false;

    static bool lockWave = false;
    static bool canGoOutside = false;

    // === MidWare ===
    static bool firstFrame = true;
    if (firstFrame)
    {
        // Set an initial size and position for the new window.
        ImGui::SetNextWindowSize(ImVec2(300, 400));
        ImGui::SetNextWindowPos(ImVec2(1000, 10));
        firstFrame = false;
    }

    // Push custom styling: extra padding and a dark background for a modern look.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.2f, 1.0f));

    ImGui::Begin("MidWare", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::IsWindowCollapsed())
    {
        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        return; // Skip drawing the rest if minimized
    }

    if (ImGui::BeginTabBar("ModMenuTabs"))
    {
        // --- WEAPON Tab ---
        if (ImGui::BeginTabItem("WEAPON"))
        {
            ImGui::Spacing();
            std::string currentWeaponNameStr = WideToNarrow(weaponName);
            ImGui::Text("Current Weapon: %s", currentWeaponNameStr.c_str());
            ImGui::Spacing();
            ImGui::Checkbox("Infinite Ammo", &weaponSettingsMap[weaponName].infiniteAmmo);
            ImGui::Spacing();
            ImGui::Checkbox("Rapid Fire", &weaponSettingsMap[weaponName].rapidFire);
            ImGui::Spacing();
            ImGui::Checkbox("InstaKill", &weaponSettingsMap[weaponName].instaKill);
            ImGui::Spacing();
            ImGui::Checkbox("Run and Shoot", &weaponSettingsMap[weaponName].runAndShoot);
            ImGui::Spacing();
            ImGui::Checkbox("Bolt Script", &weaponSettingsMap[weaponName].boltScript);
            ImGui::Spacing();
            ImGui::Checkbox("Knife Reach", &weaponSettingsMap[weaponName].knifeReach);
            ImGui::Spacing();
            ImGui::Text("Spread Reduction Percentage");
            ImGui::SliderInt("##Spread Slider", &weaponSettingsMap[weaponName].spreadReduction, 0, 100);
            ImGui::Spacing();
            ImGui::Text("Recoil Reduction Percentage");
            ImGui::SliderInt("##Recoil Slider", &weaponSettingsMap[weaponName].recoilReduction, 0, 100);
            ImGui::Spacing();
            ImGui::Text("Weapon Caliber");
            if (ImGui::BeginCombo("##Caliber Picker", caliberTypes[weaponSettingsMap[weaponName].caliberIndex], ImGuiComboFlags_HeightLargest))
            {
                for (int i = 0; i < IM_ARRAYSIZE(caliberTypes) - 1; i++)
                {
                    bool isSelected = (selectedCaliber == i);
                    if (ImGui::Selectable(caliberTypes[i], isSelected))
                        weaponSettingsMap[weaponName].caliberIndex = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::Spacing();
            ImGui::EndTabItem();
        }

        // --- PLAYER Tab ---
        if (ImGui::BeginTabItem("PLAYER"))
        {
            ImGui::Spacing();
            ImGui::Checkbox("God Mode", &godMode);
            ImGui::Spacing();
            ImGui::Checkbox("NoClip", &noClip);
            ImGui::Spacing();
            ImGui::Checkbox("Infinite Gadgets", &infiniteGadgets);
            ImGui::Spacing();
            ImGui::Text("Player Speed Multiplier");
            static float sliderSpeed = 1.0f;
            ImGui::SliderFloat("##Speed Slider", &sliderSpeed, 1.0f, 5.0f);
            ImGui::Spacing();
            ImGui::EndTabItem();
        }

        // --- VISUAL Tab ---
        if (ImGui::BeginTabItem("VISUAL"))
        {
            ImGui::Spacing();
            ImGui::Checkbox("Glow ESP", &glowESP);
            ImGui::Spacing();
            ImGui::Checkbox("Remove Sky", &removeSky);
            ImGui::Spacing();
            ImGui::Text("FOV");
            static float sliderFOV = 1.3f;
            ImGui::SliderFloat("##FOV Slider", &sliderFOV, 1.0f, 5.0f);
            static float sliderGunFOV = 1.7f;
            ImGui::SliderFloat("##GunFOV Slider", &sliderGunFOV, 1.0f, 5.0f);
            ImGui::Spacing();
            ImGui::Text("Camera Offsets");
            static float sliderCamPosX = 0.0f;
            ImGui::SliderFloat("##CamX Slider", &sliderCamPosX, 0.0f, 10.0f);
            static float sliderCamPosY = 0.0f;
            ImGui::SliderFloat("##CamY Slider", &sliderCamPosY, 0.0f, 10.0f);
            static float sliderCamPosZ = 0.0f;
            ImGui::SliderFloat("##CamZ Slider", &sliderCamPosZ, 0.0f, 10.0f);
            static float sliderCamPosA = 0.0f;
            ImGui::SliderFloat("##CamA Slider", &sliderCamPosA, 0.0f, 10.0f);
            ImGui::Spacing();
            ImGui::EndTabItem();
        }

        // --- DRONE Tab ---
        if (ImGui::BeginTabItem("DRONE"))
        {
            ImGui::Spacing();
            ImGui::Checkbox("Twitch Jump / No Cooldown", &canJump);
            ImGui::Spacing();
            ImGui::Checkbox("Infinite Ammo", &droneAmmo);
            ImGui::Spacing();
            ImGui::Checkbox("Infinite Range", &droneRange);
            ImGui::Spacing();
            ImGui::Checkbox("Rapid Fire", &droneRapidFire);
            ImGui::Spacing();
            ImGui::Text("Drone Gravity");
            static float sliderDroneGravity = 1.0f;
            ImGui::SliderFloat("##Drone Gravity Slider", &sliderDroneGravity, 0.0f, 2.0f);
            ImGui::Text("Drone Speed Multiplier");
            static float sliderDroneSpeed = 1.0f;
            ImGui::SliderFloat("##Drone Speed Slider", &sliderDroneSpeed, 1.0f, 5.0f);
            ImGui::Spacing();
            ImGui::EndTabItem();
        }

        // --- T-HUNT Tab ---
        if (ImGui::BeginTabItem("T-HUNT"))
        {
            ImGui::Spacing();
            ImGui::Checkbox("Lock Wave", &lockWave);
            ImGui::Spacing();
            ImGui::Checkbox("Can Go Outside", &canGoOutside);
            ImGui::Spacing();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    // Footer text for a polished finish.
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.9f, 1.0f), "Made by @ midly :3");
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void EndGUIFrame(ID3D11DeviceContext* context, ID3D11RenderTargetView* target)
{
    ImGui::Render();
    context->OMSetRenderTargets(1, &target, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
