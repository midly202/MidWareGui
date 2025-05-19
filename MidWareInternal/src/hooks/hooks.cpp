#include "hooks.h"
#include "../gui/gui.h"
#include "../utils/helpers.h"
#include "kiero.h"

Present oPresent = nullptr;
HWND window = nullptr;
WNDPROC oWndProc = nullptr;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* mainRenderTargetView = nullptr;

extern bool shouldUnload;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (shouldUnload)
        return oPresent(pSwapChain, SyncInterval, Flags); // skip rendering after unload flag is set

    static bool init = false;
    if (!init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
        {
            pDevice->GetImmediateContext(&pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = sd.OutputWindow;

            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
            pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();

            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
            InitGUI(window, pDevice, pContext);
            init = true;
        }
        else
        {
            return oPresent(pSwapChain, SyncInterval, Flags);
        }
    }

    BeginGUIFrame();
    RenderGUI();
    EndGUIFrame(pContext, mainRenderTargetView);

    return oPresent(pSwapChain, SyncInterval, Flags);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Let ImGui handle the input
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    // Check if ImGui wants to block input from reaching the game
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse || io.WantCaptureKeyboard)
        return true; // Block input from reaching the game

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void Cleanup()
{
    if (mainRenderTargetView) { mainRenderTargetView->Release(); mainRenderTargetView = nullptr; }
    if (pContext) { pContext->Release(); pContext = nullptr; }
    if (pDevice) { pDevice->Release(); pDevice = nullptr; }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (oWndProc && window)
        SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)oWndProc);

    kiero::shutdown();
}
