#include "Pch.h"
#include "Composition.h"

/********************************************************************************
*
*	Device类的实现
*
********************************************************************************/
bool Device::flag = false;
ComPtr<IDCompositionDesktopDevice> Device::dskDevice;
ComPtr<IDCompositionDevice3> Device::effDevice;
ComPtr<ID2D1Factory> Device::d2dFactory;

void Device::CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevelSupported;
	ComPtr<ID3D11Device> d3dDevice;
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, d3dDevice.GetAddressOf(), &featureLevelSupported, nullptr);

	ComPtr<IDXGIDevice> dxgiDevice;
	d3dDevice.As(&dxgiDevice);

	ComPtr<ID2D1Device> d2dDevice;
	D2D1CreateDevice(dxgiDevice.Get(), D2D1::CreationProperties(D2D1_THREADING_MODE_SINGLE_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS), &d2dDevice);
	d2dDevice->GetFactory(&d2dFactory);

	DCompositionCreateDevice3(d2dDevice.Get(), IID_PPV_ARGS(&dskDevice));
	dskDevice.As(&effDevice);

	flag = true;
}

ComPtr<IDCompositionDesktopDevice> Device::GetDevice()
{
	if (!flag)
		CreateDevice();
	return dskDevice;
}

ComPtr<IDCompositionDevice3> Device::GetEffectFactory()
{
	if (!flag)
		CreateDevice();
	return effDevice;
}

ComPtr<ID2D1Factory> Device::GetD2dFactory()
{
	if (!flag)
		CreateDevice();
	return d2dFactory;
}



/********************************************************************************
*
*	Target类的实现
*
********************************************************************************/
HRESULT Target::BindToWindow(HWND hWnd)
{
	auto d = Device::GetDevice();
	return d->CreateTargetForHwnd(hWnd, TRUE, &target);
}

HRESULT Target::SetRootVisual(Visual& root)
{
	return target->SetRoot(root.Get());
}

/********************************************************************************
*
*	一些有用的函数
*
********************************************************************************/

//
HRESULT MakeRelation(Visual& visual, Surface& surface)
{
	return visual->SetContent(surface.Get());
}

///<summary>创建一个基本的IDCompositonVirtualSurface</summary>
HRESULT CreateSurface(UINT width, UINT height, Surface& surface)
{
	return Device::GetDevice()->CreateVirtualSurface(width, height, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ALPHA_MODE_PREMULTIPLIED, &surface);
}