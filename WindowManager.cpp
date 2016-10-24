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
	ID windowsKey;
	V_RETURN(mpWindows->lock(&windowsKey));
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
	V_RETURN(mpWindows->unlock(windowsKey));
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

HRESULT Window::setHWND(HWND hwnd) { 
	if (hwnd) { 
		mHWND = hwnd; 
		return S_OK; 
	}; 
	return ERROR_SUCCESS; 
};

Window::~Window() { 
	if (mHWND) { 
		DestroyWindow(mHWND); 
	}; 
};

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EventManager* pEventManager = EventManager::get();
	switch (msg)
	{
	case WM_CLOSE:
		if (pEventManager) {
			pEventManager->queueEvent(EVENT_RELEASE, NULL);
		}
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
	V_RETURN(mpWindows->unlock(0));
	
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

	Window* pWindow = new Window();
	pWindow->setWNDTitle(pWindowCreator->mWNDTitle);
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
	HRESULT hr = ERROR_SUCCESS;
	WindowCreator* pWindowCreator = (WindowCreator*)pData;
	WindowManager* pWindowManager = WindowManager::get();
	if (pWindowManager) {
		hr = pWindowManager->createWindow(pWindowCreator, &id);
	}
	if (SUCCEEDED(hr)) {
		EventManager* pEventManager = EventManager::get();
		if (pEventManager) {
			WindowCreated* pWindowCreated = new WindowCreated(pWindowCreator->mWNDTitle, id);
			hr = pEventManager->queueEvent(EVENT_WINDOWCREATED, pWindowCreated);
			if (FAILED(hr)) {
				delete pWindowCreated;
			}
		}
	}
	return hr;
};

HRESULT WM_WinParamsListener::handle(MemManaged * pData) { 
	WindowManager* pWindowManager = WindowManager::get();
	if (pWindowManager) {
		return pWindowManager->evalWinParams((WinParams*)pData);
	}
	return ERROR_SUCCESS;
};
