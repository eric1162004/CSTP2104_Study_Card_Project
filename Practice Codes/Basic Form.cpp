#include < windows.h>
#include <stdlib.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define GENERATE_BUTTON 4

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);

HMENU hMenu;
HWND hName, hAge, hOut;

int WINAPI	 WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	// MessageBox(NULL, L"Hello", L"First mesage", MB_OK);
	WNDCLASS wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc)) return -1;

	CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		case FILE_MENU_NEW:
			MessageBeep(MB_ICONINFORMATION);
			break;
		case GENERATE_BUTTON:

			wchar_t name[30], age[10], out[50];

			GetWindowText(hName, name, 30);
			GetWindowText(hAge, age, 10);

			wcscpy_s(out, name);
			wcscat_s(out, L" is ");
			wcscat_s(out, age);
			wcscat_s(out, L" years old.");

			SetWindowTextW(hOut, out);

			break;
		}
		break;

	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}

};

void AddMenus(HWND hWnd)
{
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hSubMenu, MF_STRING, NULL, L"Sub Menu Item");

	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, L"New");
	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Open Sub-menu");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
	AppendMenu(hMenu, MF_STRING, NULL, L"Help");

	SetMenu(hWnd, hMenu);
};

void AddControls(HWND hWnd)
{
	CreateWindowW(L"static", L"Name: ", WS_VISIBLE | WS_CHILD, 100, 50, 98, 38, hWnd, NULL, NULL, NULL, NULL);

	hName = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 50, 98, 38, hWnd, NULL, NULL, NULL, NULL);

	CreateWindowW(L"static", L"Age: ", WS_VISIBLE | WS_CHILD, 100, 90, 98, 38, hWnd, NULL, NULL, NULL, NULL);

	hAge = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 90, 98, 38, hWnd, NULL, NULL, NULL, NULL);

	CreateWindowW(L"button", L"Generate", WS_VISIBLE | WS_CHILD, 150, 140, 98, 38, hWnd, (HMENU)GENERATE_BUTTON, NULL, NULL, NULL);

	hOut = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 200, 300, 200, hWnd, NULL, NULL, NULL, NULL);




}