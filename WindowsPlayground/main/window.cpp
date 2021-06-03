#include "window.h"
#include <assert.h>
#include <limits>
#include <dwrite.h>
#include "resource.h"

MainWindow::MainWindow(HINSTANCE hInstance)
	:mWindow{
	(Window::registerClass(), hInstance), // this syntax means to do the registerClass first, then use hInstance
	[this](Window& w) {onCreate(w); },
	[this](Window& w, UINT message, WPARAM wParam, LPARAM lParam) { return processMessage(w.getHwnd(), message, wParam, lParam); }
} {}

// Initialize Resources
void MainWindow::onCreate(Window& w)
{
	{
		ID2D1Factory* pD2DFactory{ nullptr };
		HRESULT hr = ::D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&pD2DFactory
		);

		assert(SUCCEEDED(hr));

		// mD2DFactory is a comPtr
		mD2DFactory = pD2DFactory;
	}

	{
		IDWriteFactory* pDWriteFactory{ nullptr };
		HRESULT hr = ::DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pDWriteFactory)
		);

		assert(SUCCEEDED(hr));

		mDWriteFactory = pDWriteFactory;
	}

	{
		IDWriteTextFormat* pTextFormat;

		HRESULT hr = mDWriteFactory->CreateTextFormat(
			L"Verdana",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			50,
			L"", //locale
			&pTextFormat
		);

		assert(SUCCEEDED(hr));

		mTextFormat = pTextFormat;
		// Center the text horizontally and vertically.
		mTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	{
		::GetClientRect(w.getHwnd(), &mClientRect);

		ID2D1HwndRenderTarget* pRT{ nullptr };
		HRESULT hr = mD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				w.getHwnd(),
				D2D1::SizeU(
					mClientRect.right - mClientRect.left,
					mClientRect.bottom - mClientRect.top)
			),
			&pRT
		);

		assert(SUCCEEDED(hr));
		mRenderTarget = pRT;
	}

	{
		ID2D1SolidColorBrush* pBlackBrush{ nullptr };
		mRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&pBlackBrush
		);
		mBlackBrush = pBlackBrush;
	}

	// Open the FunDialog
	// auto value{ FunDialog::getValue(w.getHInstance(), w.getHwnd()) };
}

LRESULT MainWindow::processMessage(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
) {
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		TextOut(hdc, 5, 5, L"Hello, World!", 20);

		EndPaint(hWnd, &ps);

		hButton = CreateWindow(L"button", L"Label",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			100, 100,
			100, 100,
			hWnd, (HMENU)BUTTON_ID,
			GetModuleHandle(NULL), NULL);

		//if (!hButton)
		//{
		//	MessageBox(NULL,
		//		L"Call to CreateWindow failed!",
		//		L"Win32 Guided Tour",
		//		NULL);

		//	return 1;
		//}
	}
	break;

	case WM_CLOSE:
		::PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		mX = LOWORD(lParam);
		mY = HIWORD(lParam);
		break;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void MainWindow::frame() {
	//if (mMarginGrowing) {
	//	if (mMargin >= 65.f)
	//		mMarginGrowing = false;
	//	else
	//		mMargin += 1.f;
	//}
	//else
	//{
	//	if (mMargin <= 5.f)
	//		mMarginGrowing = true;
	//	else
	//		mMargin -= 1.f;
	//}

	mRenderTarget->BeginDraw();
	mRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::LightBlue));

	//mRenderTarget->DrawRectangle(
	//	D2D1::RectF(
	//		mClientRect.left + mMargin,
	//		mClientRect.top + mMargin,
	//		mClientRect.right - mMargin,
	//		mClientRect.bottom - mMargin),
	//	mBlackBrush);

	mRenderTarget->DrawRectangle(
		D2D1::RectF(
			mClientRect.left + 10,
			mClientRect.top + 10,
			mClientRect.right - 10,
			mClientRect.bottom - 10),
		mBlackBrush);

	static const WCHAR sc_studyAppTitle[] = L"Study Card App";
	D2D1_SIZE_F renderTargetSize = mRenderTarget->GetSize();
	mRenderTarget->DrawText(
		sc_studyAppTitle,
		ARRAYSIZE(sc_studyAppTitle) - 1,
		mTextFormat,
		D2D1::RectF(0, 20, renderTargetSize.width, renderTargetSize.height),
		mBlackBrush
	);

	float radius = 15.0f;
	mRenderTarget->DrawEllipse(D2D1::Ellipse({ FLOAT(mX), FLOAT(mY) }, radius, radius), mBlackBrush, 2.0f);

	HRESULT hr = mRenderTarget->EndDraw();

}