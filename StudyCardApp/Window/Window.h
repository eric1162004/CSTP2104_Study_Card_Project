#pragma once
#include <Windows.h>
#include <functional>

class Window
{
public:
	// call before the window appear
	using OnCreate = std::function<void(Window&)>;

	// regular msg processing
	using OnMessage = std::function < LRESULT(
		Window&, UINT message, WPARAM wParam, LPARAM lParam)>;

	Window(
		HINSTANCE hInst,
		const wchar_t* className, const wchar_t* windowTitle,
		OnCreate onCreate, OnMessage onMessage);

	static LRESULT CALLBACK
		WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void RegisterWindowClass();
	void CreateNewWindow(
		int x, int y, int width, int height,
		HWND hwndParent = NULL, HMENU hMenu = NULL,
		HINSTANCE hInstance = NULL, LPVOID lparam = NULL);

	HWND getWindow() const { return m_hWnd; };
	HINSTANCE getInstance() const { return m_hInt; };

protected:
	HWND m_hWnd{ nullptr };
	HINSTANCE m_hInt{};
	OnCreate m_OnCreate;
	OnMessage m_OnMessage;
	const wchar_t* m_className{};
	const wchar_t* m_windowTitle{};
};

