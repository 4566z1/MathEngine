#pragma once
#include <d3d11.h>

namespace D3D
{
	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();

	inline ID3D11Device* g_pd3dDevice;
	inline ID3D11DeviceContext* g_pd3dDeviceContext;
	inline IDXGISwapChain* g_pSwapChain;
	inline ID3D11RenderTargetView*  g_mainRenderTargetView;
};