#include "StudyDialog.h"

StudyDialog::StudyDialog()
{
	RegisterClass();
};

LRESULT CALLBACK StudyDialog::DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case REMOVE_FROM_LOOP_BUTTON:
			MessageBox(hWnd, L"REMOVE_FROM_LOOP_BUTTON", L"Message", MB_OK);
			break;
		case SHOW_BUTTON:
			MessageBox(hWnd, L"SHOW_BUTTON", L"Message", MB_OK);
			break;
		case NEXT_BUTTON:
			MessageBox(hWnd, L"NEXT_BUTTON", L"Message", MB_OK);
			break;
		}
		break;

	case WM_CREATE:
	{
		CreateWindowW(L"static", L"Apple", WS_VISIBLE | WS_CHILD, 80, 60, 350, 30, hWnd, NULL, NULL, NULL, NULL);

		CreateWindowW(L"static", L"The round fruit of a tree of the rose family", WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_BORDER, 80, 100, 350, 120, hWnd, NULL, NULL, NULL, NULL);

		HWND hOpenButton = CreateWindowW(
			L"button", L"Remove item",
			WS_VISIBLE | WS_CHILD,
			80, 240, 110, 40,
			hWnd, (HMENU)REMOVE_FROM_LOOP_BUTTON, NULL, NULL, NULL);

		HWND hEditButton = CreateWindowW(
			L"button", L"Show Definition",
			WS_VISIBLE | WS_CHILD,
			200, 240, 110, 40,
			hWnd, (HMENU)SHOW_BUTTON, NULL, NULL, NULL);

		HWND hDeleteButton = CreateWindowW(
			L"button", L"Next Item",
			WS_VISIBLE | WS_CHILD,
			320, 240, 110, 40,
			hWnd, (HMENU)NEXT_BUTTON, NULL, NULL, NULL);
	}
	break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

void StudyDialog::DisplayDialog(HWND hWnd)
{
	m_hDlg = CreateWindowEx(
		0, L"StudyDialog", L"My ABC Card Set",
		WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		200, 100, 500, 400, hWnd, NULL, NULL, this);
}

void StudyDialog::RegisterClass()
{
	WNDCLASSEX dialog = { 0 };
	dialog.cbSize = sizeof(WNDCLASSEX);
	dialog.lpfnWndProc = DialogProcedure;
	dialog.hInstance = ::GetModuleHandle(NULL);
	dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
	dialog.hCursor = LoadCursor(NULL, IDC_ARROW);
	dialog.lpszClassName = L"StudyDialog";

	const ATOM registered = ::RegisterClassEx(&dialog);
	if (!registered)
	{
		DWORD err{ ::GetLastError() };
		assert(err == ERROR_CLASS_ALREADY_EXISTS);

		LPWSTR messageBuffer{ nullptr };
		DWORD size{ ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL) };
		::LocalFree(messageBuffer);
	}
}

