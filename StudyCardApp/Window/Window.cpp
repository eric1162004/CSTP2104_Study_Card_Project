#include "window.h"
#include "assert.h"
#include <functional>
#include <limits>

Window::Window(
	HINSTANCE hInst,
	const wchar_t* className, const wchar_t* windowTitle,
	OnCreate onCreate, OnMessage onMessage) :
	m_hInt{ hInst },
	m_className{ className },
	m_windowTitle{ windowTitle },
	m_OnCreate{ std::move(onCreate) },
	m_OnMessage{ std::move(onMessage) }
{}

LRESULT CALLBACK
Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* window =
		reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (window) return window->m_OnMessage(*window, message, wParam, lParam);

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		window = reinterpret_cast<Window*>(createStruct->lpCreateParams);

		::SetLastError(0);
		const LONG_PTR prevValue = ::SetWindowLongPtr(
			hWnd, GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
		const DWORD lastError = GetLastError();

		window->m_hWnd = hWnd;
		window->m_OnCreate(*window);

		return 0;
	}

	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::RegisterWindowClass()
{
	HINSTANCE module = ::GetModuleHandle(NULL);

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = module;
	wc.lpszClassName = m_className;
	wc.lpfnWndProc = WindowProc;

	// Register Window
	const ATOM registered = ::RegisterClassEx(&wc);
	if (!registered)
	{
		DWORD err{ ::GetLastError() };
		assert(err == ERROR_CLASS_ALREADY_EXISTS);

		LPWSTR messageBuffer{ nullptr };
		DWORD size{ ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL) };
		::LocalFree(messageBuffer);
	}
}

void Window::CreateNewWindow(
	int x, int y, int width, int height,
	HWND hwndParent, HMENU hMenu,
	HINSTANCE hInstance, LPVOID lparam)
{
	// Create the window
	m_hWnd = CreateWindowEx(
		0, m_className, m_windowTitle,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		x, y, width, height,
		hwndParent, hMenu, hInstance, lparam);
}

