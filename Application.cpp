#include "Pch.h"
#include "Application.h"

void Application::InitializeDComp()
{
	TCHAR text[] = L"type: button.           \nname: lucy.\nage : 14";
	CreateToolTipForControl(m_hBtn, text);
	TCHAR t2[] = L"mouse move";
	m_hTracking = CreateTrackingToolTip(m_hWnd, t2);
}

HWND Application::CreateToolTipForControl(HWND hControl, PTSTR szText)
{
	HWND hToolTip = CreateWindowEx(
		0, TOOLTIPS_CLASS, NULL, TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hControl, NULL, m_hInstance, NULL);

	TOOLINFO toolInfo = { sizeof(TOOLINFO) };
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	toolInfo.hwnd = hControl;
	toolInfo.uId = (UINT_PTR)hControl;
	toolInfo.lpszText = szText;

	SendMessage(hToolTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	SendMessage(hToolTip, TTM_SETMAXTIPWIDTH, 0, 200);
	return hToolTip;
}

void Application::CreateToolTipForRect(HWND hwndParent, PTSTR szText)
{
	HWND htt = CreateWindowEx(0, TOOLTIPS_CLASS, NULL, TTS_BALLOON, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hwndParent, NULL, m_hInstance, NULL);

	TOOLINFO ti = { sizeof(TOOLINFO) };
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = hwndParent;
	ti.lpszText = szText;
	GetClientRect(hwndParent, &ti.rect);
	SendMessage(htt, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
}

HWND Application::CreateTrackingToolTip(HWND hControl, PTSTR szText)
{
	HWND htt = CreateWindowEx(0, TOOLTIPS_CLASS, NULL, 0,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hControl, NULL, m_hInstance, NULL);
	m_toolInfo = { sizeof(TOOLINFO) };
	m_toolInfo.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
	m_toolInfo.hwnd = hControl;
	m_toolInfo.lpszText = szText;
	m_toolInfo.uId = (UINT_PTR)hControl;
	GetClientRect(hControl, &m_toolInfo.rect);
	SendMessage(htt, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&m_toolInfo);
	SendMessage(htt, TTM_SETTITLE, (WPARAM)TTI_INFO_LARGE, (LPARAM)L"Coordinate");

	return htt;
}
