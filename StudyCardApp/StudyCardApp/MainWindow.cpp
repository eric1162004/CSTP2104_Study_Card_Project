#include "Window/Window.h"
#include "MainWindow.h"
#include "resource.h"
#include <fileManager.h>

LRESULT MainWindow::processMessage(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case NEW_CARD_SET_BUTTON:
			CreateNewFile();
			RefreshListBox();
			break;

		case OPEN_BUTTON:
			if (m_selectedFile[0] == L'\0') break;

			m_StudyDialog.SetFileName(m_selectedFile);
			m_StudyDialog.DisplayDialog(hWnd);
			break;

		case EDIT_BUTTON:
			if (m_selectedFile[0] == L'\0') break;

			m_EditorDialog.SetFileName(m_selectedFile);
			m_EditorDialog.DisplayDialog(hWnd);
			break;

		case DELETE_BUTTON:
			RemoveFileFromList(m_currentFileIndex);
			RefreshListBox();
			break;

		case MAIN_LISTBOX:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				int lbItem = (int)SendMessage(m_hList, LB_GETCURSEL, 0, 0);
				m_currentFileIndex = (int)SendMessage(m_hList, LB_GETITEMDATA, lbItem, 0);
				m_selectedFile = m_files[m_currentFileIndex].c_str();
				break;
			}
		}
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

	m_hList = CreateWindowW(L"listbox", L"List box",
		WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD | LBS_NOTIFY,
		90, 140, 160, 150, hWnd, (HMENU)MAIN_LISTBOX, NULL, NULL);

	RefreshListBox();

	CreateWindowW(L"static", L"My Study Cards ",
		WS_VISIBLE | WS_CHILD,
		200, 50, 200, 30, hWnd, NULL, NULL, NULL, NULL);

	CreateWindowW(L"static", L"New File Name:",
		WS_VISIBLE | WS_CHILD,
		90, 90, 150, 20, hWnd, NULL, NULL, NULL, NULL);

	m_hNewFileName = CreateWindowW(L"edit", L"",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		90, 110, 150, 20, hWnd, NULL, NULL, NULL, NULL);

	HWND hNewCardSetButton = CreateWindowW(
		L"button", L"Add New File",
		WS_VISIBLE | WS_CHILD,
		280, 90, 150, 40,
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
		50, 50, LR_LOADFROMFILE);

	m_hLogo = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
		120, 35, 50, 50, hWnd, NULL, NULL, NULL, NULL);
	SendMessageW(m_hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)m_hLogoImage);
}

void MainWindow::RemoveFileFromList(int index)
{
	if (index < m_files.size())
	{
		FileManager::DeleteCardFile(FileManager::WideString2String(m_selectedFile));
		m_files.erase(m_files.begin() + index);
		m_selectedFile = L"";
	}
	else
		MessageBeep(1);
}

void MainWindow::CreateNewFile()
{
	TCHAR fileName[1024];
	GetWindowText(m_hNewFileName, fileName, 1024);

	if (fileName[0] == L'\0')
	{
		MessageBeep(1);
		return;
	}

	std::vector<std::wstring> items{};
	FileManager::CreateCardFile(fileName);

	SetWindowText(m_hNewFileName, L"");
}

void MainWindow::ClearListBox()
{
	SendMessage(m_hList, LB_RESETCONTENT, NULL, NULL);
}

void MainWindow::RefreshListBox()
{
	ClearListBox();
	m_files.clear();

	FileManager::GetListOfCardFiles(m_files);

	for (int i = 0; i < (int)m_files.size(); i++)
	{
		int pos = (int)SendMessage(m_hList, LB_ADDSTRING, 0,
			(LPARAM)m_files[i].c_str());
		SendMessage(m_hList, LB_SETITEMDATA, pos, (LPARAM)i);
	}

	SetFocus(m_hList);
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
