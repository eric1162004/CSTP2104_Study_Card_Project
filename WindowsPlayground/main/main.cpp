#include "window.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	MainWindow window(hInstance);

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else {
			window.frame();
		}
	}

	return 0;
}