#pragma once
#include "Window/Window.h"
#include "resource.h"
#include <assert.h>

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

private:
	HWND m_hDlg;
	HWND m_hList;
	HWND m_hOpenButton;
	HWND m_hEditButton;
	HWND m_hDeleteButton;


};

