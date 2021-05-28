#include "window.h"
#include "windowsx.h"
#include <assert.h>
// #include <limits>

// #include <clrWrapper/wrapper.h>

const wchar_t* cWndClassName{ L"ExampleWinApp" };

LRESULT CALLBACK
Window::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* window =
		reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (window) // if there is a window, process the mesage
	{
		return window->processMessage(hWnd, message, wParam, lParam);
	}
	else if (message == WM_CREATE) // if there is no window, and the message is WM_CREATE
	{
		LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		window = reinterpret_cast<Window*>(createStruct->lpCreateParams);

		::SetLastError(0);
		const LONG_PTR prevValue = ::SetWindowLongPtr(
			hWnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
		const DWORD lastError = ::GetLastError();
		assert(prevValue != 0 || lastError == 0);

		window->onCreate(hWnd);

		return 0;
	}
	else // Calls the default window procedure to provide default processing for any window
	{
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}
}

//  Window Constructor
Window::Window(HINSTANCE hInstance)
{
	::CreateWindowEx(
		0,
		cWndClassName,
		L"Example",
		(WS_VISIBLE | WS_OVERLAPPEDWINDOW) & ~WS_THICKFRAME,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600,
		nullptr, // parent window
		nullptr, // menu
		hInstance,
		this);
}

// Here where we create the D2DFactor, RenderTarget, ColorBrush
void Window::onCreate(HWND hWnd)
{
	mHwnd = hWnd; // set the hWnd handle to private field

	{
		ID2D1Factory* pD2DFactory{ nullptr };
		HRESULT hr = ::D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&pD2DFactory
		);
		assert(SUCCEEDED(hr));

		// Is a ComPtr
		mD2DFactory = pD2DFactory;

		// No need to release because ComPtr class takes care of it
		// pD2DFactory->Release();
	}

	{
		// Obtain the size of the drawing area.
		::GetClientRect(mHwnd, &mClientRect);

		// Create a Direct2D render target
		ID2D1HwndRenderTarget* pRT{ nullptr };
		HRESULT hr = mD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				mHwnd,
				D2D1::SizeU(
					mClientRect.right - mClientRect.left, //width of the RenderTarget
					mClientRect.bottom - mClientRect.top) // height of the RenderTarget
			),
			&pRT  // pointer to be fill up by the CreateHwndRenderTarget
		);
		assert(SUCCEEDED(hr));
		mRenderTarget = pRT;  // set the return pointer to private field
	}

	{
		ID2D1SolidColorBrush* pBlackBrush{ nullptr };
		mRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&pBlackBrush  // pointer to be filled 
		);
		mBlackBrush = pBlackBrush;
	}
}

LRESULT Window::processMessage(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		::PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		// mX = lParam & std::numeric_limits<uint16_t>::max();
		// mY = lParam >> 0x10;
		mX = GET_X_LPARAM(lParam);
		mY = GET_Y_LPARAM(lParam);
		break;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::frame()
{
	if (mMarginGrowing)
	{
		if (mMargin >= 65.f)
			mMarginGrowing = false;
		else
			mMargin += 1.f;
	}
	else
	{
		if (mMargin <= 5.f)
			mMarginGrowing = true;
		else
			mMargin -= 1.f;
	}

	mRenderTarget->BeginDraw();
	mRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Aquamarine));

	// Draw the moving Rectangle
	mRenderTarget->DrawRectangle(
		D2D1::RectF(
			mClientRect.left + mMargin,
			mClientRect.top + mMargin,
			mClientRect.right - mMargin,
			mClientRect.bottom - mMargin),
		mBlackBrush);

	// Draw the circle 
	// float radius = getRadius(0); // from C# .DLL
	float radius = 15.0f;
	mRenderTarget->DrawEllipse(D2D1::Ellipse({ FLOAT(mX), FLOAT(mY) }, radius, radius), mBlackBrush);

	HRESULT hr = mRenderTarget->EndDraw();
}

//void Window::GetModuleHandle(void*)
//{
//
//}

void Window::registerClass()
{
	HINSTANCE module = ::GetModuleHandle(NULL);

	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = windowProc;
	windowClass.hInstance = module;
	windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = cWndClassName;
	const ATOM registered = ::RegisterClassEx(&windowClass);
	assert(registered);
}
