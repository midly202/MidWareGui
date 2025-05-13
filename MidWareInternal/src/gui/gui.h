#pragma once
#include <Windows.h>
#include <d3d11.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void InitGUI(HWND window, ID3D11Device* device, ID3D11DeviceContext* context);
void BeginGUIFrame();
void RenderGUI();
void EndGUIFrame(ID3D11DeviceContext* context, ID3D11RenderTargetView* target);
