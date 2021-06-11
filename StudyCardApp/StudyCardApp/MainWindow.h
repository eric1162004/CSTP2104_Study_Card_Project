#pragma once
#include <Windows.h>
#include "Window/Window.h"
#include "./EditorDialog.h"
#include "./StudyDialog.h"

class MainWindow
{
public:
	MainWindow(HINSTANCE hInstance);

private:
	void onCreate(Window&);

	LRESULT  processMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	Window m_window;
	EditorDialog m_EditorDialog;
	StudyDialog m_StudyDialog;

	HBITMAP m_hLogoImage;
	HWND m_hLogo;
};

