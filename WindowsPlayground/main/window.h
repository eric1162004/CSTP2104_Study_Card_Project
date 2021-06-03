#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <cstdint>
#include "../windowLib/comPtr.h"
#include "../windowLib/window.h"
#include "fun_dialog.h"
#include <dwrite.h>

class MainWindow
{
public:
	MainWindow(HINSTANCE hInstance);

	void frame();

private:
	void onCreate(Window&);

	LRESULT processMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	RECT mClientRect;
	ComPtr<ID2D1Factory> mD2DFactory;
	ComPtr<IDWriteFactory > mDWriteFactory;
	ComPtr<IDWriteTextFormat> mTextFormat;
	ComPtr<ID2D1HwndRenderTarget> mRenderTarget;
	ComPtr<ID2D1SolidColorBrush> mBlackBrush;

	float mMargin{ 5.0f };
	bool mMarginGrowing{ true };

	// Mouse Position
	uint32_t mX{ 0 };
	uint32_t mY{ 0 };

	Window mWindow; // Owner 
	FunDialog mDialog; // child
	HWND hButton;

};