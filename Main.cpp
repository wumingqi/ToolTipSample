#include "Pch.h"
#include "Application.h"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT nCmdShow)
{
	CoInitialize(nullptr);
	InitCommonControls();
	Application(960, 640, hInstance).Run(nCmdShow);
	//Application(960, 640, hInstance).Run(nCmdShow);
	return Application::EnterMessageLoop();
}