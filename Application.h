#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

class Application
{
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	HWND m_hBtn;
	HWND m_ht1, m_ht2, m_hTracking;
	UINT m_width, m_height;

	TOOLINFO m_toolInfo;
	BOOL m_bTrackingMouse;

	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT Handle_WM_CREATE(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_PAINT(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_SIZE(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_RBUTTONDOWN(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_RBUTTONUP(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_MOUSEMOVE(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_MOUSELEAVE(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_MOUSEWHEEL(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_KEYDOWN(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_DESTROY(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_DROPFILES(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_COMMAND(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_NCHITTEST(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_NCCALCSIZE(WPARAM wParam, LPARAM lParam);


	ComPtr<ID2D1Device>					m_d2dDevice;
	ComPtr<IDCompositionDesktopDevice>	m_dcompDevice;
	ComPtr<IDCompositionTarget>			m_dcompTarget;
	ComPtr<IDCompositionVisual2>		m_dcompVisual;
	ComPtr<IDCompositionVirtualSurface> m_dcompSurface;
	void InitializeDComp();

	HWND CreateToolTipForControl(HWND hControl, PTSTR szText);
	void CreateToolTipForRect(HWND hwndParent, PTSTR szText);
	HWND CreateTrackingToolTip(HWND hControl, PTSTR szText);

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int EnterMessageLoop();
	Application(UINT width, UINT height, HINSTANCE hInstance) :
		m_width(width),
		m_height(height),
		m_hInstance(hInstance),
		m_bTrackingMouse(FALSE)
	{}
	virtual ~Application()
	{
	}

	int Run(int nCmdShow)
	{
		WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
		wc.style = CS_VREDRAW | CS_HREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = L"HelloWindows";
		wc.hIconSm = nullptr;

		RegisterClassEx(&wc);

		DWORD style = WS_OVERLAPPEDWINDOW; DWORD styleEx = 0;
		RECT rc = { 0,0,(LONG)m_width,(LONG)m_height };
		AdjustWindowRectEx(&rc, style, false, styleEx);
		auto cx = GetSystemMetrics(SM_CXSCREEN);
		auto cy = GetSystemMetrics(SM_CYFULLSCREEN);
		auto w = rc.right - rc.left;
		auto h = rc.bottom - rc.top;
		auto x = (cx - w) / 2;
		auto y = (cy - h) / 2;

		m_hWnd = CreateWindowEx(0,
			wc.lpszClassName, L"ToolTipSample", WS_OVERLAPPEDWINDOW,
			x, y, w, h, nullptr, nullptr, m_hInstance, this);

		m_hBtn = CreateWindowEx(0, WC_BUTTON, L"Beauty", WS_VISIBLE | WS_CHILD, 
			50, 50, 200, 50, m_hWnd, (HMENU)101, m_hInstance, nullptr);

		InitializeDComp();
		ShowWindow(m_hWnd, nCmdShow);

		return 0;
	}
};

#endif // !APPLICATION_H
