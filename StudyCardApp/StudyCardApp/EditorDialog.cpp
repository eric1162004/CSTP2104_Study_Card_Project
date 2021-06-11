#include "EditorDialog.h"

EditorDialog::EditorDialog()
{
	RegisterClass();
};

LRESULT CALLBACK EditorDialog::DialogProcedure(
	HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	EditorDialog* window =
		reinterpret_cast<EditorDialog*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (window)
	{
		return window->OnMessage(*window, msg, wp, lp);
	}
	else if (msg == WM_CREATE)
	{
		LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lp);
		window = reinterpret_cast<EditorDialog*>(createStruct->lpCreateParams);

		const LONG_PTR prevValue = ::SetWindowLongPtr(
			hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));

		window->m_hDlg = hWnd;
		window->OnCreate();

		return 0;
	}
	else
	{
		return ::DefWindowProc(hWnd, msg, wp, lp);
	}
}

LRESULT EditorDialog::OnMessage(
	EditorDialog& editorDialog, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case ADD_ITEM_BUTTON:
			MessageBox(m_hDlg, L"ADD_ITEM_BUTTON", L"Message", MB_OK);
			break;
		case DELETE_ITEM_BUTTON:
			MessageBox(m_hDlg, L"DELETE_ITEM_BUTTON", L"Message", MB_OK);
			break;
		case SAVE_EXIT_BUTTON:
			MessageBox(m_hDlg, L"SAVE_EXIT_BUTTON", L"Message", MB_OK);
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(m_hDlg);
		break;

	default:
		return DefWindowProc(m_hDlg, msg, wp, lp);
	}
}

void EditorDialog::OnCreate()
{
	HWND hList = CreateWindowW(
		L"listbox", L"List box", WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 90, 160, 200, m_hDlg, (HMENU)EDITOR_LISTBOX, NULL, NULL);
	SendMessageW(hList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Hello"));
	SendMessageW(hList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Hello2"));

	CreateWindowW(
		L"static", L"Edit Your Card Set", WS_VISIBLE | WS_CHILD, 190, 50, 200, 30, m_hDlg, NULL, NULL, NULL, NULL);

	CreateWindowW(L"static", L"Title:", WS_VISIBLE | WS_CHILD, 280, 90, 150, 20, m_hDlg, NULL, NULL, NULL, NULL);

	CreateWindowW(L"edit", L"My ABC Card Set", WS_VISIBLE | WS_CHILD | WS_BORDER, 280, 110, 150, 20, m_hDlg, NULL, NULL, NULL, NULL);

	HWND hOpenButton = CreateWindowW(
		L"button", L"Add Item",
		WS_VISIBLE | WS_CHILD,
		280, 140, 150, 40,
		m_hDlg, (HMENU)ADD_ITEM_BUTTON, NULL, NULL, NULL);

	HWND hEditButton = CreateWindowW(
		L"button", L"Delete Item",
		WS_VISIBLE | WS_CHILD,
		280, 190, 150, 40,
		m_hDlg, (HMENU)DELETE_ITEM_BUTTON, NULL, NULL, NULL);

	HWND hDeleteButton = CreateWindowW(
		L"button", L"Save and Exit",
		WS_VISIBLE | WS_CHILD,
		280, 240, 150, 40,
		m_hDlg, (HMENU)SAVE_EXIT_BUTTON, NULL, NULL, NULL);
}

void EditorDialog::DisplayDialog(HWND hWnd)
{
	m_hDlg = CreateWindowEx(0, L"EditorDialog", L"Card Set Editor",
		WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		200, 100, 500, 400, hWnd, NULL, NULL, this);
}

void EditorDialog::RegisterClass()
{
	WNDCLASSEX dialog = { 0 };
	dialog.cbSize = sizeof(WNDCLASSEX);
	dialog.lpfnWndProc = DialogProcedure;
	dialog.hInstance = ::GetModuleHandle(NULL);
	dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
	dialog.hCursor = LoadCursor(NULL, IDC_ARROW);
	dialog.lpszClassName = L"EditorDialog";

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

