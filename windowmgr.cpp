#include "TopInclude.h"
#include "Engine.h"
#include "EventManager.h"
#include "Memory.h"

#include "windowmgr.h"

WindowManager* gpWindowManager;

LRESULT CALLBACK WindowManager::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CE1_ASSERT(hwnd);
	switch (msg)
	{
	case WM_CLOSE:
		EventManager::get()->queueEvent(EVENT_RELEASE,NULL,false);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0; 
}

HRESULT WindowManager::createWindow() {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = Engine::getWinParams()->mHInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"myWindowClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc); 
	mHWND = CreateWindowEx(WS_EX_CLIENTEDGE, wc.lpszClassName, mWNDTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, mWNDWidth, mWNDHeight, NULL, NULL, mHINSTANCE, NULL);

	ShowWindow(mHWND, mCMDShow);
	UpdateWindow(mHWND);
}

WindowManager::WindowManager()
{
	EventManager::get()->registerForEvent(EVENT_RESTORE,&restore);
	EventManager::get()->registerForEvent(EVENT_RELEASE,&release);
}

HRESULT WindowManager::run(TIME elapsed) {
	
	MSG msg;
	while (PeekMessage(&msg, WindowManager::get()->mHWND, 0, 0, PM_REMOVE))
	{
		switch (msg.message)
		{
			default:
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	return S_OK;
}

WindowManager * WindowManager::get()
{
	if (!gpWindowManager) {
		gpWindowManager = new WindowManager();
	}
	return gpWindowManager;
}

HRESULT WindowManager::restore(void* p0)
{
	get()->createWindow();
	return S_OK;
}
