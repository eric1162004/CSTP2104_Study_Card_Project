#include "MainWindow.h"
#include "Window/Window.h"
#include "resource.h"

MainWindow::MainWindow(HINSTANCE hInstance)
	: m_window{
		hInstance, L"MainWindow", L"Study Card Application",
		[this](Window& w) { onCreate(w); },
		[this](Window& w, UINT message, WPARAM wParam, LPARAM lParam)
		{ return processMessage(w.getWindow(), message, wParam, lParam); } }
{
	m_window.RegisterWindowClass();
	m_window.CreateNewWindow(100, 100, 500, 400, NULL, NULL, hInstance, this);
}

void MainWindow::onCreate(Window& w)
{
	HWND hWnd = w.getWindow();

	CreateWindowW(L"static", L"My Study Cards ",
		WS_VISIBLE | WS_CHILD,
		200, 50, 200, 30, hWnd, NULL, NULL, NULL, NULL);

	HWND hNewCardSetButton = CreateWindowW(
		L"button", L"New Card Set",
		WS_VISIBLE | WS_CHILD,
		150, 80, 200, 40,
		hWnd, (HMENU)1, NULL, NULL, NULL);

	HWND hOpenButton = CreateWindowW(
		L"button", L"Open",
		WS_VISIBLE | WS_CHILD,
		280, 140, 150, 40,
		hWnd, (HMENU)1, NULL, NULL, NULL);

	HWND hEditButton = CreateWindowW(
		L"button", L"Edit",
		WS_VISIBLE | WS_CHILD,
		280, 190, 150, 40,
		hWnd, (HMENU)1, NULL, NULL, NULL);

	HWND hDeleteButton = CreateWindowW(
		L"button", L"Delete",
		WS_VISIBLE | WS_CHILD,
		280, 240, 150, 40,
		hWnd, (HMENU)1, NULL, NULL, NULL);
}

LRESULT MainWindow::processMessage(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (wParam) {
		case 1:
			MessageBox(hWnd, L"Hello World", L"Title", MB_OK);
		}
		break;

	case WM_CLOSE:
		::PostQuitMessage(0);
		break;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

