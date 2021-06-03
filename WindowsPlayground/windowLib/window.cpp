#include "window.h"
#include <assert.h>
#include <limits>
#include "fileWrapper/wrapper.h" // testing

const wchar_t* cWndClassName{ L"ExampleWinApp" };

LRESULT CALLBACK
Window::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// testing function from fileWrapper
	// float testNum = getFileNum(0);

	Window* window = reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (window)
	{
		return window->mOnMessage(*window, message, wParam, lParam);
	}
	else if (message == WM_CREATE)
	{
		//Defines the initialization parameters passed to the window procedure of an application. These
		// members are identical to the parameters of the CreateWindowEx function.
		LPCREATESTRUCT	createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		window = reinterpret_cast<Window*>(createStruct->lpCreateParams);

		// Sets the last-error code for the calling thread.
		::SetLastError(0);

		// Changes an attribute of the specified window.
		const LONG_PTR preValue = ::SetWindowLongPtr(
			hWnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams)
		);

		const DWORD lastError = ::GetLastError();
		assert(preValue != 0 || lastError == 0);

		window->mHwnd = hWnd;
		window->mOnCreate(*window);

		return 0;
	}
	else
	{
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

}

HINSTANCE Window::getHInstance() const
{
	return mHInstance;
}

HWND Window::getHwnd() const
{
	return mHwnd;
}

Window::Window(HINSTANCE hInstance, OnCreate onCreate, OnMessage onMessage) :
	mOnCreate{ std::move(onCreate) },
	mOnMessage{ std::move(onMessage) },
	mHInstance{ hInstance }
{
	// Creates an overlapped, pop - up, or child window with an extended window style; 
	// otherwise, this function is identical to the CreateWindow function.
	::CreateWindowEx(
		0, // The extended window style of the window being created.
		cWndClassName, //  Specifies the window class name. The class name can be any name registered with RegisterClass 
		L"Study Card App", // name displayed in the title bar.
		(WS_VISIBLE | WS_OVERLAPPEDWINDOW) & ~WS_THICKFRAME, //The style of the window being created.
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800, // width
		600, // height
		nullptr, //A handle to the parent or owner window of the window being created. 
		nullptr, // A handle to a menu, or specifies a child-window identifier
		hInstance, //A handle to the instance of the module to be associated with the window.
		this // Pointer to a value to be passed to the window through the CREATESTRUCT structure 
	);
}

void Window::registerClass() {
	HINSTANCE module = ::GetModuleHandle(NULL); //Retrieves a module handle for the specified module. 

	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX); // The size, in bytes, of this structure.
	windowClass.lpfnWndProc = windowProc; // A pointer to the window procedure. 
	windowClass.hInstance = module; // A handle to the instance that contains the window procedure for the class.
	windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = cWndClassName; // specifies the window class name.

	const ATOM registered = ::RegisterClassEx(&windowClass); //Registers a window class

	if (!registered)
	{
		DWORD err{ ::GetLastError() };
		assert(err == ERROR_CLASS_ALREADY_EXISTS);

		LPWSTR messageBuffer{ nullptr };
		// Formats a message string.
		DWORD size{ ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL) };
		// Frees the specified local memory objectand invalidates its handle.
		::LocalFree(messageBuffer);
	}

}