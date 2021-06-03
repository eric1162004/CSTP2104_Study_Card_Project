#include "customButton.h"
#include <commctrl.h>

void CustomButton::assignButton(HWND button)
{
	// Changes an attribute of the specified window with Get/SetWindowLong
	// GWL_STYLE - Sets a new window style.
	LONG style{ ::GetWindowLong(button, GWL_STYLE) };
	style |= BS_OWNERDRAW;
	::SetWindowLong(button, GWL_STYLE, style);

	// put 'this' in the window's user data area
	const  LONG_PTR prevValue{ ::SetWindowLongPtr(
		button,
		GWLP_USERDATA,
		reinterpret_cast<LONG_PTR>(this)
	) };

	// set the button to a private member
	mHwnd = button;

	// Retrieves the coordinates of a window's client area. 
	// &mRect - A pointer to a RECT structure that receives the client coordinates
	::GetClientRect(button, &mRect);

	// Installs or updates a window subclass callback.
	::SetWindowSubclass(button, CustomButton::buttonProc, 0, 0);
}

LRESULT CALLBACK CustomButton::buttonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	CustomButton* button{
		reinterpret_cast<CustomButton*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA))
	};

	return button->processMsg(uMsg, wParam, lParam);
}

LRESULT CustomButton::processMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_PAINT:
	{
		// retrieves a handle to a device context (DC) for the client area of a specified window or for the entire screen
		HDC hdc{ ::GetDC(mHwnd) };
		HPEN hpenDot{ ::CreatePen(PS_DASHDOTDOT, 5, RGB(250,90,60)) };
		::SelectObject(hdc, hpenDot);
		::Rectangle(hdc, mRect.left, mRect.top, mRect.right, mRect.bottom);

		HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Roboto Th");
		SelectObject(hdc, hFont);
		SetTextColor(hdc, RGB(100, 170, 255));
		TextOut(hdc, 10, 10, L"Welcome", strlen("Welcome"));

		::ReleaseDC(mHwnd, hdc);
		::DeleteObject(hpenDot);
		return TRUE;
	}
	}

	return ::DefSubclassProc(mHwnd, uMsg, wParam, lParam);
}



