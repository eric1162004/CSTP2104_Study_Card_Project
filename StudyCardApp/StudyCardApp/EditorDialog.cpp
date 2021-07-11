#include "EditorDialog.h"


LRESULT EditorDialog::OnMessage(
	EditorDialog& editorDialog, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case ADD_ITEM_BUTTON:
			AddWordItem();
			RefreshListBox();
			break;
		case DELETE_ITEM_BUTTON:
			RemoveItemFromItems(m_curentItemIndex);
			RefreshListBox();
			break;
		case SAVE_EXIT_BUTTON:
			SaveItems();
			break;
		case EDITOR_LISTBOX:
		{
			switch (HIWORD(wp))
			{
			case LBN_SELCHANGE:
			{
				int lbItem = (int)SendMessage(m_hList, LB_GETCURSEL, 0, 0);
				m_curentItemIndex = (int)SendMessage(m_hList, LB_GETITEMDATA, lbItem, 0);
				break;
			}
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

void EditorDialog::AddWordItem()
{
	TCHAR keywordBuff[1024];
	GetWindowText(m_hKeyword, keywordBuff, 1024);

	TCHAR definitionBuff[1024];
	GetWindowText(m_hDefinition, definitionBuff, 1024);

	if (keywordBuff[0] == L'\0' || definitionBuff[0] == L'\0')
	{
		MessageBeep(1);
		return;
	}

	std::wstring keyword{ keywordBuff };
	std::wstring definition{ definitionBuff };

	m_Items.push_back({ keyword, definition, keyword.append(L":").append(definition) });

	SetWindowText(m_hKeyword, L"");
	SetWindowText(m_hDefinition, L"");
}

void EditorDialog::RemoveItemFromItems(int index)
{
	if (index < m_Items.size())
		m_Items.erase(m_Items.begin() + index);
	else
		MessageBeep(1);
}

void EditorDialog::ClearListBox()
{
	SendMessage(m_hList, LB_RESETCONTENT, NULL, NULL);
}

void EditorDialog::RefreshListBox()
{
	ClearListBox();

	for (int i = 0; i < (int)m_Items.size(); i++)
	{
		int pos = (int)SendMessage(m_hList, LB_ADDSTRING, 0,
			(LPARAM)m_Items[i].fullName.c_str());

		SendMessage(m_hList, LB_SETITEMDATA, pos, (LPARAM)i);
	}
}

void EditorDialog::OnCreate()
{
	m_Items.clear();

	m_hList = CreateWindowW(L"listbox", L"List box",
		WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD | LBS_NOTIFY,
		90, 140, 160, 150, m_hDlg, (HMENU)EDITOR_LISTBOX, NULL, NULL);

	std::vector<std::wstring> wordItems = {};
	std::string fileName = FileManager::WideString2String(m_fileName);
	FileManager::GetItemsFromCardFile(fileName, wordItems);

	int itemsSize = wordItems.size();
	for (int i = 0; i < itemsSize; i++)
	{
		std::wstringstream ss;
		ss.str(wordItems[i].c_str());

		std::wstring keyword;
		std::wstring definition;

		std::getline(ss, keyword, L':');
		std::getline(ss, definition);

		FileManager::ItemStruct itemStruct;
		itemStruct.keyword = keyword;
		itemStruct.definition = definition;
		itemStruct.fullName = keyword.append(L":").append(definition);

		m_Items.push_back(itemStruct);
	}

	RefreshListBox();

	SetFocus(m_hList);

	CreateWindowW(L"static", L"Edit Your Card Set",
		WS_VISIBLE | WS_CHILD,
		190, 50, 200, 30, m_hDlg, NULL, NULL, NULL, NULL);

	CreateWindowW(L"static", L"Keyword",
		WS_VISIBLE | WS_CHILD,
		90, 90, 150, 20, m_hDlg, NULL, NULL, NULL, NULL);

	m_hKeyword = CreateWindowW(L"edit", L"",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		90, 110, 150, 20, m_hDlg, NULL, NULL, NULL, NULL);

	CreateWindowW(L"static", L"Definition",
		WS_VISIBLE | WS_CHILD,
		280, 90, 150, 20, m_hDlg, NULL, NULL, NULL, NULL);

	m_hDefinition = CreateWindowW(L"edit", L"",
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
		L"button", L"Save",
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

void EditorDialog::SaveItems()
{
	std::vector<std::wstring> itemsToSave = {};

	for (auto item : m_Items)
	{
		itemsToSave.push_back(item.fullName);
	}

	FileManager::WriteToCardFile(m_fileName, itemsToSave);
};

void EditorDialog::SetFileName(std::wstring fileName)
{
	m_fileName = fileName;
};

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



