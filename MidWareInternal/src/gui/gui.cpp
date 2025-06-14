#include "../utils/helpers.h"
#include "../features/visual.h"
#include "../features/thunt.h"
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
    extern std::wstring displayWeaponName;

    extern uintptr_t baseAddress;
    extern CheatSettings cheatSettings;

    bool knifeReachBugged = false;

    static int selectedCaliber = 0;
    const char* caliberTypes[] = {
        "Bullet_SuperLow",
        "Bullet_Low",
        "Bullet_Mid",
        "Bullet_High",
        "Bullet_SuperHigh",
        "Bullet_MaxPower",
        "Bullet_BOSG",
        "Bullet_CSRX300",
        "Bullet_DMR",
        "Bullet_DP27",
        "Shell_Low",
        "Shell_Mid",
        "Shell_High",
        "Gadget_BarrageTurret",
        "Gadget_BlowTorch",
        " "
    };

    static bool infiniteGadgets = false;    // dont remove
    static bool canGoOutside = false;       // dont remove
    static bool droneNoGravity = false;
    static bool droneSpeed = false;

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
            std::string currentWeaponNameStr = WideToNarrow(displayWeaponName);
            ImGui::Text("Current Weapon: %s", currentWeaponNameStr.c_str());
            ImGui::Spacing();
            ImGui::Checkbox("Infinite Ammo", &weaponSettingsMap[displayWeaponName].infiniteAmmo);
            ImGui::Spacing();
            ImGui::Checkbox("No Recoil", &weaponSettingsMap[displayWeaponName].noRecoil);
            ImGui::Spacing();
            ImGui::Checkbox("Full Auto", &weaponSettingsMap[displayWeaponName].fullAuto);
            ImGui::Spacing();
            ImGui::Checkbox("Rapid Fire", &weaponSettingsMap[displayWeaponName].rapidFire);
            ImGui::Spacing();
            ImGui::Checkbox("InstaKill", &weaponSettingsMap[displayWeaponName].instaKill);
            ImGui::Spacing();
            if (ImGui::Checkbox("Run & Shoot", &cheatSettings.runShoot))
                cheatSettings.toggleRunShoot = true;
            ImGui::Spacing();
            if (ImGui::Checkbox("Bolt Script", &cheatSettings.boltScript))
                cheatSettings.toggleBoltScript = true;
            ImGui::Spacing();
            /*
            uintptr_t knifePtr = GetPointer(baseAddress, offsets::Knife);
            if (knifePtr && knifePtr > 0x10000) 
            {
                Knife* knife = reinterpret_cast<Knife*>(knifePtr);

                if ((knife->knifeDistancePlayers > 1.2 && knife->knifeDistancePlayers < 1.4) ||
                    knife->knifeDistancePlayers == 9999 &&
                    (knife->knifeDistanceWalls >= 1.2 && knife->knifeDistanceWalls < 1.4 ||
                        knife->knifeDistanceWalls == 9999))
                    ImGui::Checkbox("Knife Reach", &cheatSettings.knifeReach);

                else 
                {
                    if (ImGui::Checkbox("Knife Reach", &knifeReachBugged)) 
                        knifeReachBugged = false;
                    if (ImGui::IsItemHovered()) 
                        ImGui::SetTooltip("This feature is unavailable right now.\n\nWant to know why? Go to the README on github.");
                }
            }
            else 
            {
                if (ImGui::Checkbox("Knife Reach", &knifeReachBugged)) 
                    knifeReachBugged = false;
                if (ImGui::IsItemHovered()) 
                    ImGui::SetTooltip("This feature is unavailable right now.\n\nWant to know why? Go to the README on github.");
            }
            */
            ImGui::Checkbox("Knife Reach", &cheatSettings.knifeReach);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("This feature is unstable and will not work in some game sessions.");
            ImGui::Spacing();
            ImGui::Text("Spread Reduction Percentage");
            ImGui::SliderInt("##Spread Slider", &weaponSettingsMap[displayWeaponName].spreadReduction, 0, 100);
            ImGui::Spacing();
            ImGui::Text("Weapon Caliber");
            int& caliberIndex = weaponSettingsMap[displayWeaponName].caliberIndex;
            if (ImGui::BeginCombo("##Caliber Picker", caliberTypes[caliberIndex], ImGuiComboFlags_HeightLargest))
            {
                for (int i = 0; i < IM_ARRAYSIZE(caliberTypes) - 1; i++)
                {
                    bool isSelected = (caliberIndex == i);  // Compare to actual weapon setting
                    if (ImGui::Selectable(caliberTypes[i], isSelected))
                        caliberIndex = i;  // Update directly
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
            ImGui::Checkbox("God Mode", &cheatSettings.godMode);
            ImGui::Spacing();
            ImGui::Checkbox("NoClip", &cheatSettings.noClip);
            ImGui::Spacing();
            if (ImGui::Checkbox("Infinite Gadgets", &infiniteGadgets))
                cheatSettings.toggleInfGadgets = true;
            ImGui::Spacing();
            ImGui::Text("Player Speed Multiplier");
            ImGui::SliderFloat("##Speed Slider", &cheatSettings.playerSpeed, 1.0f, 5.0f);
            ImGui::Spacing();
            ImGui::EndTabItem();
        }

        // --- VISUAL Tab ---
        if (ImGui::BeginTabItem("VISUAL"))
        {
            ImGui::Spacing();
            if (ImGui::Checkbox("Glow ESP", &cheatSettings.glowESP))
                cheatSettings.toggleGlowESP = true;
            ImGui::Spacing();
            ImGui::Checkbox("Remove Sky", &cheatSettings.removeSky);
            ImGui::Spacing();
            ImGui::Text("Player FOV");
            ImGui::SliderFloat("##FOV Slider", &cheatSettings.playerFOV, 1.0f, 3.0f);
            ImGui::Text("Weapon FOV");
            ImGui::SliderFloat("##GunFOV Slider", &cheatSettings.gunFOV, 0.0f, 5.0f);
            ImGui::Spacing();
            if (ImGui::TreeNode("Camera Settings"))
            {
                if (ImGui::Button("Reset Settings"))
                    camReset();
                ImGui::Spacing();
                ImGui::Text("Position");
                ImGui::SliderFloat("##CamX", &cheatSettings.camX, -5.0f, 5.0f);
                ImGui::SliderFloat("##CamY", &cheatSettings.camY, -5.0f, 5.0f);
                ImGui::SliderFloat("##CamZ", &cheatSettings.camZ, -5.0f, 5.0f);
                ImGui::Spacing();
                ImGui::Text("Orientation");
                ImGui::SliderFloat("##CamPitch", &cheatSettings.camPitch, -1.0f, 1.0f);
                ImGui::SliderFloat("##CamRoll", &cheatSettings.camRoll, -1.0f, 1.0f);
                ImGui::SliderFloat("##CamYaw", &cheatSettings.camYaw, -1.0f, 1.0f);
                ImGui::Spacing();
                ImGui::Text("Near Clipping Plane");
                ImGui::SliderFloat("##CamClip", &cheatSettings.camDistance, 0.0f, 50.0f);
                ImGui::Spacing();
                ImGui::Text("Camera FOV");
                ImGui::SliderFloat("##CamFOV", &cheatSettings.camFOV, -1.0f, 1.0f);
                
                ImGui::TreePop();
            }
            //ImGui::Spacing();
            ImGui::EndTabItem();
        }

        // --- DRONE Tab ---
        if (ImGui::BeginTabItem("DRONE"))
        {
            ImGui::Spacing();
            ImGui::Checkbox("Drone Jump", &cheatSettings.droneJump);
            ImGui::Spacing();
            ImGui::Checkbox("No Jump Cooldown", &cheatSettings.droneJumpCooldown);
            ImGui::Spacing();
            ImGui::Checkbox("Infinite Ammo", &cheatSettings.droneAmmo);
            ImGui::Spacing();
            ImGui::Checkbox("Infinite Range", &cheatSettings.droneRange);
            ImGui::Spacing();
            ImGui::Text("Drone Gravity");
            ImGui::SliderFloat("##Drone Gravity Slider", &cheatSettings.droneGravity, 0.01f, 2.0f);
            ImGui::Text("Drone Speed Multiplier");
            ImGui::SliderFloat("##Drone Speed Slider", &cheatSettings.droneSpeed, 1.0f, 5.0f);
            ImGui::Spacing();
            ImGui::EndTabItem();
        }

        // --- T-HUNT Tab ---
        if (ImGui::BeginTabItem("MISC"))
        {
            ImGui::Spacing();
            if (ImGui::Button("End Round"))
                gameState(5);
            ImGui::Spacing();
            if (ImGui::Button("End Match"))
                gameState(2);
            ImGui::Spacing();
            ImGui::Checkbox("Infinite Time", &cheatSettings.infiniteTime);
            ImGui::Spacing();
            ImGui::Checkbox("Lock Wave", &cheatSettings.lockWave);
            ImGui::Spacing();
            if (ImGui::Checkbox("Can Go Outside", &canGoOutside))
                cheatSettings.toggleGoOutside = true;
            ImGui::Spacing();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    // Footer text for a polished finish.
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.9f, 1.0f), "Made by Midly :3");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Discord: @midly\nGithub:  @midly202");
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
