#include "MainWindow.h"
#include "Window/Window.h"
#include "resource.h"

LRESULT MainWindow::processMessage(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:

		switch (wParam)
		{
		case NEW_CARD_SET_BUTTON:
			m_EditorDialog.DisplayDialog(hWnd);
			break;

		case OPEN_BUTTON:
			m_StudyDialog.DisplayDialog(hWnd);
			break;

		case EDIT_BUTTON:
			MessageBox(hWnd, L"Edit", L"Title", MB_OK);
			break;

		case DELETE_BUTTON:
			MessageBox(hWnd, L"Delete this card set?", L"Delete Card Set", MB_YESNO | MB_ICONWARNING);
			break;
		}
		break;

	case WM_CLOSE:
		::PostQuitMessage(0);
		break;
	}

	return ::DefWindowProc(hWnd, message, wParam, lParam);
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
		hWnd, (HMENU)NEW_CARD_SET_BUTTON, NULL, NULL, NULL);

	HWND hOpenButton = CreateWindowW(
		L"button", L"Open Card Set",
		WS_VISIBLE | WS_CHILD,
		280, 140, 150, 40,
		hWnd, (HMENU)OPEN_BUTTON, NULL, NULL, NULL);

	HWND hEditButton = CreateWindowW(
		L"button", L"Edit",
		WS_VISIBLE | WS_CHILD,
		280, 190, 150, 40,
		hWnd, (HMENU)EDIT_BUTTON, NULL, NULL, NULL);

	HWND hDeleteButton = CreateWindowW(
		L"button", L"Delete",
		WS_VISIBLE | WS_CHILD,
		280, 240, 150, 40,
		hWnd, (HMENU)DELETE_BUTTON, NULL, NULL, NULL);

	m_hLogoImage = (HBITMAP)LoadImageW(NULL, L"minionLogo.bmp", IMAGE_BITMAP,
		150, 150, LR_LOADFROMFILE);

	m_hLogo = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
		90, 140, 150, 150, hWnd, NULL, NULL, NULL, NULL);
	SendMessageW(m_hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)m_hLogoImage);
}

MainWindow::MainWindow(HINSTANCE hInstance)
	: m_window{
		hInstance, L"MainWindow", L"Study Card Application",
		[this](Window& w) { onCreate(w); },
		[this](Window& w, UINT message, WPARAM wParam, LPARAM lParam)
		{ return processMessage(w.getWindow(), message, wParam, lParam); } },
	m_EditorDialog{},
	m_StudyDialog{}
{
	m_window.RegisterWindowClass();
	m_window.CreateNewWindow(100, 100, 500, 400, NULL, NULL, hInstance, this);
}
