#pragma once
#include <Windows.h>
#include "Window/Window.h"
#include "./EditorDialog.h"
#include "./StudyDialog.h"

#include <thread>
#include <mutex>
#include <Window/threadPool.h>
#include <Window/comPtr.h>
#include <d2d1.h>

#include<Window/Registry.h>

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

	void RenderD2D1Graphic(HWND hWnd);
	void RenderWelcomeMessage(HWND hWnd);

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

	RECT mClientRect{};
	ComPtr<ID2D1Factory> mD2DFactory{};
	ComPtr<ID2D1HwndRenderTarget> mRenderTarget{};
	ComPtr<ID2D1SolidColorBrush> mBlackBrush{};

	ThreadPool<2> mThreadPool{};
};

