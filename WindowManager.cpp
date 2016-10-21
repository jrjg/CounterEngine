#include "TopInclude.h"
#include "EventManager.h"

#include "WindowManager.h"

HRESULT WindowManager::evalWinParams(WinParams* pWinParams) {
	mCmdShow = pWinParams->mCmdShow;
	mHInstance = pWinParams->mHInstance;
	mHPrevInstance = pWinParams->mHPrevInstance;
	mpCmdLine = pWinParams->mpCmdLine;
	return S_OK;
}

WindowManager::~WindowManager()
{
	SAFE_RELEASE(mpWindows);
	SAFE_RELEASE(mpWinParamsListener);
	SAFE_RELEASE(mpCreateWindowListener);
}

HRESULT WindowManager::run(TIME elapsed)
{
	MSG msg;
	Window* pWindow;
	for(UINT i = 0; i<mpWindows->getIndexLast();){
		pWindow = mpWindows->get(i++);
		if (pWindow) {
			while (PeekMessage(&msg, pWindow->getHWND(), 0, 0, PM_REMOVE))
			{

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	return S_OK;
}

HRESULT WindowManager::restore()
{
	if (!mpWindows) { 
		mpWindows = new Vector<Window>(2); 
	}else {
		mpWindows->restore();
	}
	SAFE_RELEASE(mpWinParamsListener); mpWinParamsListener = new WM_WinParamsListener();
	SAFE_RELEASE(mpCreateWindowListener); mpCreateWindowListener = new CreateWindowListener();
	return S_OK;
}

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		EventManager::get()->queueEvent(EVENT_RELEASE, NULL);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

HRESULT WindowManager::createWindow(WindowCreator* pWindowCreator, UINT* pID) {
	Window* pWindow = new Window();
	pWindow->setWNDTitle(pWindowCreator->mWNDTitle);
	
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = Window::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"myWindowClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc); 

	HRESULT hr = pWindow->setHWND(CreateWindowEx(WS_EX_CLIENTEDGE, wc.lpszClassName, pWindow->getWNDTitle(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, pWindowCreator->mWNDWidth, pWindowCreator->mWNDHeight, NULL, NULL, mHInstance, NULL));
	if (SUCCEEDED(hr)) {
		ShowWindow(pWindow->getHWND(), mCmdShow);
		UpdateWindow(pWindow->getHWND());
		return mpWindows->pushback(pWindow, pID);
	}
	else {
		SAFE_RELEASE(pWindow);
	}
	return hr;
}

HRESULT CreateWindowListener::handle(MemManaged * pData) {
	UINT id;
	WindowCreator* pWindowCreator = (WindowCreator*)pData;
	HRESULT hr = WindowManager::get()->createWindow(pWindowCreator, &id);
	if (SUCCEEDED(hr)) {
		EventManager::get()->queueEvent(EVENT_WINDOWCREATED, new WindowCreated(pWindowCreator->mWNDTitle, id));
	}
	return hr;
};

HRESULT WM_WinParamsListener::handle(MemManaged * pData) { 
	return WindowManager::get()->evalWinParams((WinParams*)pData); 
};
