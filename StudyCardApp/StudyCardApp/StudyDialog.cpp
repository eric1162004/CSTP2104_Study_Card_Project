#include "StudyDialog.h"

LRESULT StudyDialog::OnMessage(
	StudyDialog& studyDialog, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case REMOVE_FROM_LOOP_BUTTON:
			MessageBox(m_hDlg, L"REMOVE_FROM_LOOP_BUTTON", L"Message", MB_OK);
			break;
		case SHOW_BUTTON:
			ShowDefinition();
			break;
		case NEXT_BUTTON:
			ShowNextWord();
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

void StudyDialog::OnCreate()
{
	RefreshListBox();

	m_hKeyword = CreateWindowW(L"static", m_keyword.c_str(),
		WS_VISIBLE | WS_CHILD,
		80, 60, 350, 30, m_hDlg, NULL, NULL, NULL, NULL);

	m_hDefinition = CreateWindowW(L"static", m_definition.c_str(),
		WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_BORDER,
		80, 100, 350, 120, m_hDlg, NULL, NULL, NULL, NULL);

	CreateWindowW(
		L"button", L"Show Definition",
		WS_VISIBLE | WS_CHILD,
		80, 240, 110, 40,
		m_hDlg, (HMENU)SHOW_BUTTON, NULL, NULL, NULL);

	CreateWindowW(
		L"button", L"Next Item",
		WS_VISIBLE | WS_CHILD,
		320, 240, 110, 40,
		m_hDlg, (HMENU)NEXT_BUTTON, NULL, NULL, NULL);
}

void StudyDialog::ShowDefinition()
{
	if (m_items.size() == 0) return;

	m_definition = m_items[m_currentItemIndex].definition;
	SetWindowText(m_hDefinition, m_definition.c_str());
}

void StudyDialog::ShowNextWord()
{
	if (m_items.size() == 0) return;

	m_currentItemIndex = ++m_currentItemIndex % m_items.size();
	m_keyword = m_items[m_currentItemIndex].keyword;
	m_definition = L"";

	SetWindowText(m_hKeyword, m_keyword.c_str());
	SetWindowText(m_hDefinition, m_definition.c_str());
}

void StudyDialog::RefreshListBox()
{
	m_items.clear();

	FileManager::GetAndParseItemsFromCardFile(
		FileManager::WideString2String(m_fileName), m_items);

	m_keyword = (m_items.size() == 0) ? L"" :
		m_items[m_currentItemIndex].keyword;
	m_definition = L"";
}

LRESULT CALLBACK StudyDialog::DialogProcedure(
	HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	StudyDialog* dialog =
		reinterpret_cast<StudyDialog*>(::GetWindowLongPtr(hDlg, GWLP_USERDATA));

	if (dialog)
	{
		return dialog->OnMessage(*dialog, msg, wp, lp);
	}
	else if (msg == WM_CREATE)
	{
		LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lp);
		dialog = reinterpret_cast<StudyDialog*>(createStruct->lpCreateParams);

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

void StudyDialog::SetFileName(std::wstring fileName)
{
	m_fileName = fileName;
};


StudyDialog::StudyDialog()
{
	RegisterClass();
};

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

void StudyDialog::DisplayDialog(HWND hWnd)
{
	m_hDlg = CreateWindowEx(
		0, L"StudyDialog", m_fileName.c_str(),
		WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		200, 100, 500, 400, NULL, NULL, NULL, this);
}


