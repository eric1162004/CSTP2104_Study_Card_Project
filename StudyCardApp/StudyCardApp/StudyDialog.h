#pragma once
#include "resource.h"
#include "Window/Window.h"

#include <assert.h>
#include <fileManager.h>

#include <thread>
#include <mutex>
#include <Window/threadPool.h>

class StudyDialog

{
public:
	LRESULT OnMessage(StudyDialog&, UINT msg, WPARAM wp, LPARAM lp);
	void OnCreate();

	StudyDialog();
	void DisplayDialog(HWND hWnd);

	static void RegisterClass();
	static LRESULT CALLBACK
		DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

	void SetFileName(std::wstring fileName);
	void ShowNextWord();
	void ShowDefinition();
	void RefreshListBox();

private:
	HWND m_hDlg;
	HWND m_hKeyword;
	HWND m_hDefinition;

	std::wstring m_fileName{};
	std::vector<FileManager::ItemStruct>  m_items = {};
	int m_currentItemIndex{};

	std::wstring m_keyword{};
	std::wstring m_definition{};

	ThreadPool<2> mThreadPool{};
};

