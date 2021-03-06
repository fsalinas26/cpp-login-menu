// Dear ImGui: standalone example application for DirectX 11
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "json.hpp"
#include <map>
#include "main.h"
#include <Windows.h>
#include <WinUser.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <string.h>
#include <d3d11.h>
#include <tchar.h>
#include "ImGuiInits.h"
#include "WndProc.h"
#include "myriad.h"
#include "HttpRequest.h"
#include <crypto.h>
#include "Authentication.h"
#include "CallbackThread.h"
#include "DrawForm.h"


extern c_crypto g_crypto;

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    g_response["res"] = "";
#ifdef DEBUG
    AllocConsole();

    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif // DEBUG_MODE
    g_crypto.key = c_crypto::b64_to_binary("oUHqJ9IOlyjA4edqmyFdkeNi8J/x+dte2AWlGRd2uTM=");
    
    HWND desktop = GetDesktopWindow();
    RECT desktopr;
    GetWindowRect(desktop, &desktopr);
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "Login Form", NULL };
    RegisterClassEx(&wc);
    HWND main_hwnd = CreateWindow(wc.lpszClassName, "Login Form", WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(main_hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);
    SetForegroundWindow(main_hwnd);
    //ImGui::CreateContext();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


    Smallfont = io.Fonts->AddFontFromMemoryCompressedTTF(Medium_compressed_data, Medium_compressed_size, 16.f);
    LargeFont = io.Fonts->AddFontFromMemoryCompressedTTF(Medium_compressed_data, Medium_compressed_size, 60.f);

    InitStyle();

    
    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    DWORD window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect);
    auto x = float(screen_rect.right) * 0.1f;
    auto y = float(screen_rect.bottom) * 0.1f;
    ImVec4 clear_color = ImVec4(0, 0, 0, 1.0f);
    InitImGui();
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5, 0.5, 0.5, 1.0));
    // Main loop
    bool done = false;
    bool activef = true;
    while (!done)
    {

        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(1000, 600));
        ImGui::SetNextWindowBgAlpha(1.0f);
        
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("##Login Form", &activef, window_flags);
        switch (currentPage)
        {
        case Login:
            LoginPage();
            break;
        case Menu:
            //Your Code Here
            break;
        default:
            break;
        }
        ImGui::End();

        ImGui::Render();

        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync

    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(main_hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

