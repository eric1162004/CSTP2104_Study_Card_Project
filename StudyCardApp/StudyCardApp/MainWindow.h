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

	void CreateNewFile();
	void ClearListBox();
	void RefreshListBox();
	void RemoveFileFromList(int index);

	LRESULT  processMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	Window m_window;
	EditorDialog m_EditorDialog;
	StudyDialog m_StudyDialog;

	HBITMAP m_hLogoImage;
	HWND m_hLogo;
	HWND m_hNewFileName;
	HWND m_hList;

	std::vector<std::wstring> m_files = {};
	int m_currentFileIndex{};

	std::wstring m_selectedFile;
	std::wstring m_newFileName;
};

