#include "fun_dialog.h"
#include <assert.h>
#include <memory>
#include "resource.h"

INT_PTR FunDialog::Dlgproc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// this msg is sent to the dialog box procedure immediately before a dialog box is displayed
	case WM_INITDIALOG:
	{
		// Retrieves a handle to a control in the specified dialog box.
		HWND button{ ::GetDlgItem(hwnd, IDC_CUSTOMBUTTON) };

		// Get the ptr of the dialog
		FunDialog* dialog{ reinterpret_cast<FunDialog*>(lParam) };

		dialog->mCustomButton.assignButton(button);

		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			// Destroys a modal dialog box, causing the system to end any processing for the dialog box.
			::EndDialog(hwnd, 10);
			return TRUE;
		case IDC_CUSTOMBUTTON:
			return TRUE;
		}
	case WM_DESTROY:
		return TRUE;
	}
	return FALSE;
}

uint64_t FunDialog::getValue(HINSTANCE hInstance, HWND parent)
{
	FunDialog dialog;
	static_assert(sizeof(&dialog) == sizeof(LPARAM));

	// Creates a modal dialog box from a dialog box template resource. 
	// DialogBoxParamW is blocking, so it's ok to pass the pointer
	uint64_t value{ static_cast<uint64_t>(::DialogBoxParamW(hInstance, MAKEINTRESOURCE(IDD_FUN_DIALOG), parent, Dlgproc, reinterpret_cast<LPARAM>(&dialog))) };

	// the user click on ok or cancel?
	return value;
}