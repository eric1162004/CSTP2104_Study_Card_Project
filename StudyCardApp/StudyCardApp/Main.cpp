#include <windows.h>
#include "MainWindow.h"

int WINAPI	 WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	MainWindow mainWindow(hInst);

	// Message Loop of the Application 
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
