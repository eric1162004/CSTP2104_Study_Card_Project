#include <windowLib/window.h>

int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	Window::registerClass();

	// Make a new instance of window
	Window window(hInstance);

	MSG msg = {};
	int counter = 0;
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			window.frame();
		}
	}

	return 0;
}