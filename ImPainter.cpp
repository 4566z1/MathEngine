#include "ImPainter.hpp"
#include "D3D.hpp"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (D3D::g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            D3D::CleanupRenderTarget();
            D3D::g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            D3D::CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

/*
 *  ----- ImPainter Class -----
 */

void ImPainter::begin()
{
	MSG msg;
    while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            m_is_done = true;
    }
	ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    m_draw_list = ImGui::GetForegroundDrawList();
}

void ImPainter::end()
{
	ImGui::Render();
    const float m_background_color_with_alpha[4] = { m_background_color.x * m_background_color.w, m_background_color.y * m_background_color.w, m_background_color.z * m_background_color.w, m_background_color.w };
	D3D::g_pd3dDeviceContext->OMSetRenderTargets(1, &D3D::g_mainRenderTargetView, NULL);
	D3D::g_pd3dDeviceContext->ClearRenderTargetView(D3D::g_mainRenderTargetView, m_background_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	D3D::g_pSwapChain->Present(1, 0); // Present with vsync
}

ImPainter::ImPainter(wstring_view window_title, 
    const int& width, const int& height,const ImVec4& background_color)
{
    m_background_color = background_color;
	m_wndclass = { sizeof(m_wndclass), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&m_wndclass);
    m_hwnd = ::CreateWindowW(m_wndclass.lpszClassName, window_title.data(), WS_OVERLAPPEDWINDOW, 100, 100, width, height, NULL, NULL, m_wndclass.hInstance, NULL);

    if (!D3D::CreateDeviceD3D(m_hwnd))
    {
        D3D::CleanupDeviceD3D();
        ::UnregisterClassW(m_wndclass.lpszClassName, m_wndclass.hInstance);
        return;
    }

    ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(m_hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(m_hwnd);
    ImGui_ImplDX11_Init(D3D::g_pd3dDevice, D3D::g_pd3dDeviceContext);
}

ImPainter::~ImPainter()
{
	ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    D3D::CleanupDeviceD3D();
    ::DestroyWindow(m_hwnd);
    ::UnregisterClassW(m_wndclass.lpszClassName, m_wndclass.hInstance);
}