#include "Pch.h"
#include "Application.h"

#define HANDLE_MSG(msg) case msg: return Handle_##msg(wParam, lParam);
LRESULT Application::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(WM_CREATE);
		//HANDLE_MSG(WM_PAINT);
		HANDLE_MSG(WM_SIZE);
		HANDLE_MSG(WM_LBUTTONDOWN);
		HANDLE_MSG(WM_LBUTTONUP);
		HANDLE_MSG(WM_RBUTTONDOWN);
		HANDLE_MSG(WM_RBUTTONUP);
		HANDLE_MSG(WM_MOUSEMOVE);
		HANDLE_MSG(WM_MOUSELEAVE);
		HANDLE_MSG(WM_MOUSEWHEEL);
		HANDLE_MSG(WM_KEYDOWN);
		HANDLE_MSG(WM_DESTROY);
		HANDLE_MSG(WM_DROPFILES);
		HANDLE_MSG(WM_COMMAND);
		//HANDLE_MSG(WM_NCHITTEST);
		//HANDLE_MSG(WM_NCCALCSIZE);
	default: return DefWindowProc(m_hWnd, msg, wParam, lParam);
	}
}
#undef HANDLE_MSG

LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Application* app = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (msg == WM_CREATE) 
	{
		LPCREATESTRUCT lpCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)lpCreateStruct->lpCreateParams);
		app = reinterpret_cast<Application*>(lpCreateStruct->lpCreateParams);
		app->m_hWnd = hWnd;
		SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	}
	
	if (app != nullptr) 
	{
		return app->HandleMessage(msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int Application::EnterMessageLoop()
{
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}

///<summary>�����ڴ���ʱ��������Ϣ</summary>
///<param name="wParam">This parameter is not used.</param>
///<param name="lParam">ָ��CREATESTRUCT�ṹ���ָ��</param>
LRESULT Application::Handle_WM_CREATE(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

///<summary>����WM_PAINT��Ϣ�����ͻ�����Ҫ������ʱ����</summary>
///<param name="wParam">This parameter is not used.</param>
///<param name="lParam">This parameter is not used.</param>
LRESULT Application::Handle_WM_PAINT(WPARAM wParam, LPARAM lParam)
{
	ValidateRect(m_hWnd, nullptr);
	return 0;
}


///<summary>����WM_LBUTTONDOWN��Ϣ������������ʱ����</summary>
///<param name="wParam">��ʾ�Ƿ������ⰴ�����£���MK_CONTROL��</param>
///<param name="lParam">����ڿͻ������ϽǵĹ�������</param>
LRESULT Application::Handle_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
	POINT pt = { LOWORD(lParam),HIWORD(lParam) };
	ClientToScreen(m_hWnd, &pt);
	RECT rc = { pt.x-50, pt.y-50, pt.x + 50,pt.y + 50 };
	GetWindowRect(m_hWnd, &rc);
	ClipCursor(&rc);
	return 0;
}


///<summary>����WM_LBUTTONUP��Ϣ���������ɿ�ʱ����</summary>
///<param name="wParam">��ʾ�Ƿ������ⰴ�����£���MK_CONTROL��</param>
///<param name="lParam">����ڿͻ������ϽǵĹ�������</param>
LRESULT Application::Handle_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam)
{
	ClipCursor(nullptr);
	return 0;
}


///<summary>����WM_RBUTTONDOWN��Ϣ������Ҽ�����ʱ����</summary>
///<param name="wParam">��ʾ�Ƿ������ⰴ�����£���MK_CONTROL��</param>
///<param name="lParam">����ڿͻ������ϽǵĹ�������</param>
LRESULT Application::Handle_WM_RBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

///<summary>����WM_RBUTTONUP��Ϣ������Ҽ��ɿ�ʱ����</summary>
///<param name="wParam">��ʾ�Ƿ������ⰴ�����£���MK_CONTROL��</param>
///<param name="lParam">����ڿͻ������ϽǵĹ�������</param>
LRESULT Application::Handle_WM_RBUTTONUP(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


///<summary>����WM_MOUSE��Ϣ������ڿͻ����ƶ�ʱ����</summary>
///<param name="wParam">��ʾ�Ƿ������ⰴ�����£���MK_CONTROL��</param>
///<param name="lParam">����ڿͻ������ϽǵĹ�������</param>
LRESULT Application::Handle_WM_MOUSEMOVE(WPARAM wParam, LPARAM lParam)
{
	if (!m_bTrackingMouse)				// �״ν���ͻ���
	{
		TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;

		TrackMouseEvent(&tme);

		SendMessage(m_hTracking, TTM_TRACKACTIVATE, TRUE, (LPARAM)&m_toolInfo);
		m_bTrackingMouse = TRUE;
	}
	POINT pt = { LOWORD(lParam),HIWORD(lParam) };
	ClientToScreen(m_hWnd, &pt);
	SendMessage(m_hTracking, TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x , pt.y + 20));
	return 0;
}

LRESULT Application::Handle_WM_MOUSELEAVE(WPARAM wParam, LPARAM lParam)
{
	SendMessage(m_hTracking, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&m_toolInfo);
	m_bTrackingMouse = FALSE;
	return 0;
}


///<summary>����WM_SIZE��Ϣ�������ڴ�С�ı�ʱ����</summary>
///<param name="wParam">��ʾ��С�ı�����ͣ���SIZE_MAXIMIZED��SIZE_HIDE��</param>
///<param name="lParam">���ڿͻ����Ĵ�С</param>
LRESULT Application::Handle_WM_SIZE(WPARAM wParam, LPARAM lParam)
{
	m_width = LOWORD(lParam); m_height = HIWORD(lParam);
	return 0;
}


///<summary>����WM_KEYDOWN��Ϣ�������̰�������ʱ����</summary>
///<param name="wParam">��ϵͳ���������ֵ��Virtual-Key Code��</param>
///<param name="lParam">������Ϣ</param>
LRESULT Application::Handle_WM_KEYDOWN(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_F1:
		break;
	case VK_F2:
		break;
	}
	return 0;
}


///<summary>����WM_MOUSEWHEEL��Ϣ���������ֹ���ʱ����</summary>
///<param name="wParam">
///���ֱ������ֹ����ķ�����ֵ������ǰ������ֵ��������
///���ֱ���ʱ����������������£�MK_CONTROL���ȣ�
///</param>
///<param name="lParam">��������λ�ã��������Ļ���Ͻ�</param>
LRESULT Application::Handle_WM_MOUSEWHEEL(WPARAM wParam, LPARAM lParam)
{
	auto flag = HIWORD(wParam) & 0x8000;
	if (LOWORD(wParam) == MK_CONTROL)
	{
		if (flag)
		{
		}
		else
		{
		}
	}
	return 0;
}

///<summary>����WM_DROPFILES��Ϣ</summary>
///<param name="wParam">һ��ָ��������ק�ļ����ڲ��ṹ��ľ����HDROP</param>
///<param name="lParam">Must be zero.</param>
LRESULT Application::Handle_WM_DROPFILES(WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop = (HDROP)wParam;
	TCHAR filename[MAX_PATH];
	UINT count = DragQueryFile(hDrop, -1, nullptr, 0);
	for (UINT i = 0; i < count; i++)
	{
		DragQueryFile(hDrop, i, filename, _countof(filename));
		//�����ȡ�����ļ���

	}
	return 0;
}

///<summary>����WM_COMMAND��Ϣ��
///1���û�ѡ��ĳ���˵���ʱ��2���ؼ�����֪ͨ��������ڡ�3�����ټ���������
///</summary>
/*
Message Source 	wParam(high word) 						wParam(low word) 				lParam
Menu 			0 										Menu identifier(IDM_*) 			0
Accelerator 	1 										Accelerator identifier(IDM_*) 	0
Control 		Control - defined notification code 	Control identifier				Handle to the control window
*/
LRESULT Application::Handle_WM_COMMAND(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

LRESULT Application::Handle_WM_NCHITTEST(WPARAM wParam, LPARAM lParam)
{
	// Get the point coordinates for the hit test.
	POINT ptMouse = { LOWORD(lParam), HIWORD(lParam) };
	// Get the window rectangle.
	RECT rcWindow;
	GetWindowRect(m_hWnd, &rcWindow);
	// Get the frame rectangle, adjusted for the style without a caption.
	RECT rcFrame = { 0 };
	AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE,
		NULL);
	// Determine if the hit test is for resizing. Default middle (1,1).
	USHORT uRow = 1;
	USHORT uCol = 1;
	bool fOnResizeBorder = false;
	// Determine if the point is at the top or bottom of the window.
	if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top +
		8)
	{
		fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
		uRow = 0;
	}
	else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom -
		8)
	{
		uRow = 2;
	}
	// Determine if the point is at the left or right of the window.
	if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left +
		8)
	{
		uCol = 0; // left side
	}
	else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right -
		8)
	{
		uCol = 2; // right side
	}
	// Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
	LRESULT hitTests[3][3] =
	{
		{ HTTOPLEFT, fOnResizeBorder ? HTTOP : HTCAPTION, HTTOPRIGHT },
		{ HTLEFT, HTCAPTION, HTRIGHT },{ HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
	};
	return hitTests[uRow][uCol];
}

LRESULT Application::Handle_WM_NCCALCSIZE(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT Application::Handle_WM_DESTROY(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}
