#include "EditorDialog.h"

typedef struct
{
	TCHAR keyword[MAX_PATH];
	TCHAR definition[MAX_PATH];
} Item;

Item Items[] =
{
	{TEXT("Haas, Jonathan"), TEXT("Midfield")},
	{TEXT("Pai, Jyothi"), TEXT("Forward") },
	{TEXT("Hanif, Kerim"), TEXT("Back") },
	{TEXT("Anderberg, Michael"), TEXT("Back")},
	{TEXT("Jelitto, Jacek"), TEXT("Midfield")},
};

LRESULT EditorDialog::OnMessage(
	EditorDialog& editorDialog, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wp))
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
		case EDITOR_LISTBOX:
		{
			switch (HIWORD(wp))
			{
			case LBN_SELCHANGE:
				int lbItem = (int)SendMessage(m_hList, LB_GETCURSEL, 0, 0);
				int i = (int)SendMessage(m_hList, LB_GETITEMDATA, lbItem, 0);
				MessageBox(NULL, Items[i].keyword, Items[i].definition, MB_OK);
				break;
			}
		}
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
	m_hList = CreateWindowW(L"listbox", L"List box",
		WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD | LBS_NOTIFY,
		90, 90, 160, 200, m_hDlg, (HMENU)EDITOR_LISTBOX, NULL, NULL);

	for (int i = 0; i < ARRAYSIZE(Items); i++)
	{
		int pos = (int)SendMessage(m_hList, LB_ADDSTRING, 0,
			(LPARAM)Items[i].keyword);
		SendMessage(m_hList, LB_SETITEMDATA, pos, (LPARAM)i);
	}
	// Set input focus to the list box.
	SetFocus(m_hList);

	CreateWindowW(L"static", L"Edit Your Card Set",
		WS_VISIBLE | WS_CHILD,
		190, 50, 200, 30, m_hDlg, NULL, NULL, NULL, NULL);

	CreateWindowW(L"static", L"Title:",
		WS_VISIBLE | WS_CHILD,
		280, 90, 150, 20, m_hDlg, NULL, NULL, NULL, NULL);

	CreateWindowW(L"edit", L"My ABC Card Set",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		280, 110, 150, 20, m_hDlg, NULL, NULL, NULL, NULL);

	m_hOpenButton = CreateWindowW(
		L"button", L"Add Item",
		WS_VISIBLE | WS_CHILD,
		280, 140, 150, 40,
		m_hDlg, (HMENU)ADD_ITEM_BUTTON, NULL, NULL, NULL);

	m_hEditButton = CreateWindowW(
		L"button", L"Delete Item",
		WS_VISIBLE | WS_CHILD,
		280, 190, 150, 40,
		m_hDlg, (HMENU)DELETE_ITEM_BUTTON, NULL, NULL, NULL);

	m_hDeleteButton = CreateWindowW(
		L"button", L"Save and Exit",
		WS_VISIBLE | WS_CHILD,
		280, 240, 150, 40,
		m_hDlg, (HMENU)SAVE_EXIT_BUTTON, NULL, NULL, NULL);
}

LRESULT CALLBACK EditorDialog::DialogProcedure(
	HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	EditorDialog* dialog =
		reinterpret_cast<EditorDialog*>(::GetWindowLongPtr(hDlg, GWLP_USERDATA));

	if (dialog)
	{
		return dialog->OnMessage(*dialog, msg, wp, lp);
	}
	else if (msg == WM_CREATE)
	{
		LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lp);
		dialog = reinterpret_cast<EditorDialog*>(createStruct->lpCreateParams);

		const LONG_PTR prevValue = ::SetWindowLongPtr(
			hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));

		dialog->m_hDlg = hDlg;
		dialog->OnCreate();

		return 0;
	}
	else
	{
		return ::DefWindowProc(hDlg, msg, wp, lp);
	}
}

EditorDialog::EditorDialog()
{
	RegisterClass();
};

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

void EditorDialog::DisplayDialog(HWND hWnd)
{
	m_hDlg = CreateWindowEx(0, L"EditorDialog", L"Card Set Editor",
		WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		200, 100, 500, 400, NULL, NULL, NULL, this);
}



