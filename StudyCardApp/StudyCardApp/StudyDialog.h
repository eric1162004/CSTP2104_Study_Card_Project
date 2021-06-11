#pragma once
#include "Window/Window.h"
#include "resource.h"
#include <assert.h>

class StudyDialog
{
public:
	static void RegisterClass();

	static LRESULT CALLBACK
		DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

	StudyDialog();
	void DisplayDialog(HWND hWnd);

private:
	HWND m_hDlg;
};

