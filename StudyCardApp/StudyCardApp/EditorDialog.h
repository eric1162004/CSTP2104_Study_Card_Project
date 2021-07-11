#pragma once
#include "Window/Window.h"
#include "resource.h"

#include <assert.h>
#include <sstream>
#include <fileManager.h>

class EditorDialog
{
public:
	LRESULT OnMessage(EditorDialog&, UINT msg, WPARAM wp, LPARAM lp);
	void OnCreate();

	EditorDialog();
	void DisplayDialog(HWND hWnd);

	static void RegisterClass();
	static LRESULT CALLBACK
		DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

	void ClearListBox();
	void RefreshListBox();

	void AddWordItem();
	void RemoveItemFromItems(int index);
	void SaveItems();

	void SetFileName(std::wstring fileName);

private:
	HWND m_hDlg;
	HWND m_hList;

	HWND m_hKeyword;
	HWND m_hDefinition;

	HWND m_hOpenButton;
	HWND m_hEditButton;
	HWND m_hDeleteButton;

	std::wstring m_fileName{};
	std::vector<FileManager::ItemStruct> m_Items = {};
	int m_curentItemIndex{};
};

