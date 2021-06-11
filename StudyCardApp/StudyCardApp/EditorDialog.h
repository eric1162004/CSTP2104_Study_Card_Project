#pragma once
#include "Window/Window.h"
#include "resource.h"
#include <assert.h>

class EditorDialog
{
public:
	static void RegisterClass();
	static LRESULT CALLBACK
		DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

	EditorDialog();
	void DisplayDialog(HWND hWnd);

	LRESULT OnMessage(EditorDialog&, UINT msg, WPARAM wp, LPARAM lp);
	void OnCreate();

private:
	HWND m_hDlg;
	int textInt{};
};

