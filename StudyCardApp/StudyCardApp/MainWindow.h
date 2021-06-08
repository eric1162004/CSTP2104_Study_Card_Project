#pragma once
#include <Windows.h>
#include "Window/Window.h"

class MainWindow
{
public:
	MainWindow(HINSTANCE hInstance);

private:
	void onCreate(Window&);

	LRESULT  processMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	Window m_window;
};

