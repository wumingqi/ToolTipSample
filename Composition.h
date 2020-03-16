#pragma once
#ifndef COMPOSITION_H
#define COMPOSITION_H

using DesktopDevice = ComPtr<IDCompositionDesktopDevice>;
using Device3		= ComPtr<IDCompositionDevice3>;
using Visual		= ComPtr<IDCompositionVisual2>;
using Surface		= ComPtr<IDCompositionVirtualSurface>;

#define DECLEAR_VISUAL_AND_SURFACE(name) Visual name;Surface  name##_s;void update_##name##();

class Device
{
	Device();
	static DesktopDevice dskDevice;
	static Device3 effDevice;
	static ComPtr<ID2D1Factory> d2dFactory;
	static void CreateDevice();
	static bool flag;
public:
	static DesktopDevice GetDevice();
	static Device3 GetEffectFactory();
	static ComPtr<ID2D1Factory> GetD2dFactory();
};

class Target
{
	ComPtr<IDCompositionTarget> target;
public:
	Target() :target(nullptr) {}
	HRESULT BindToWindow(HWND hWnd);
	HRESULT SetRootVisual(Visual& root);
};

HRESULT MakeRelation(Visual& visual, Surface& surface);
HRESULT CreateSurface(UINT width, UINT height, Surface& surface);

#endif // !COMPOSITION_H
