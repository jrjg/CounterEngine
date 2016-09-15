#include "TopInclude.h"
#include "Engine.h"
#include "EventManager.h"
#include "windowmgr.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CEASSERT(hwnd);
	switch (msg)
	{
	case WM_CLOSE:
		EventManager_QueueEvent(EVENT_END,NULL);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0; 
}

HRESULT registerWindowClass(WNDCLASSEX* pwc, HINSTANCE* phInstance) {
	CEASSERT(pwc&&phInstance);
	pwc->cbSize = sizeof(WNDCLASSEX);
	pwc->style = 0;
	pwc->lpfnWndProc = WndProc;
	pwc->cbClsExtra = 0;
	pwc->cbWndExtra = 0;
	pwc->hInstance = (*phInstance);
	pwc->hIcon = LoadIcon(NULL, IDI_APPLICATION);
	pwc->hCursor = LoadCursor(NULL, IDC_ARROW);
	pwc->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	pwc->lpszMenuName = NULL;
	pwc->lpszClassName = L"myWindowClass";
	pwc->hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	return S_OK;
}

HRESULT createWindow(WindowMgr* pWindowMgr) {
	CEASSERT(pWindowMgr);
	WNDCLASSEX* pwc;
	_NEW(WNDCLASSEX, pwc);
	SAFECALL(registerWindowClass(pwc, &(Engine_GetWinParams()->hInstance)));
	RegisterClassEx(pwc); 
	(*pWindowMgr->phwnd) = CreateWindowEx(WS_EX_CLIENTEDGE, pwc->lpszClassName, Engine_WINDOWTITLE(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Engine_BUFFERWIDTH(), Engine_BUFFERHEIGHT(), NULL, NULL, Engine_GetWinParams()->hInstance, NULL);
	return S_OK;
}

HRESULT showWindow(WindowMgr* pWindowMgr) {
	CEASSERT(pWindowMgr);
	ShowWindow(*pWindowMgr->phwnd, Engine_GetWinParams()->nCmdShow);
	UpdateWindow(*pWindowMgr->phwnd);
	return S_OK;
}

HRESULT WindowMgr_NEW(WindowMgr** ppInst) {
	_NEW(WindowMgr, *ppInst);
	WindowMgr* pInst = *ppInst;
	_NEW(HWND, pInst->phwnd);
	pInst->windowtitle = LPCWSTR(Engine_WINDOWTITLE());
	SAFECALL(createWindow(pInst));
	SAFECALL(showWindow(pInst));
	return S_OK;
}


HRESULT WindowMgr_Run(TIME elapsed) {
	WindowMgr* pWindowMgr = Engine_GetWindowMgr();
	CEASSERT(elapsed&&pWindowMgr);
	MSG msg;
	while (PeekMessage(&msg, *(pWindowMgr->phwnd), 0, 0, PM_REMOVE))
	{
		switch (msg.message)
		{
			case WM_KEYDOWN:
			{
				WPARAM* pWParam;
				_NEW(WPARAM, pWParam);
				(*pWParam) = msg.wParam;
				EventManager_QueueEvent(EVENT_KEYDOWN, (void*)pWParam);
			}
			default:
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	return S_OK;
}

HRESULT WindowMgr_DELETE(void) {
	WindowMgr* pWindowMgr = Engine_GetWindowMgr();
	CEASSERT(pWindowMgr);
	SAFECALL(DestroyWindow(*(pWindowMgr->phwnd)));
	_DEL(pWindowMgr->phwnd);
	_DEL(pWindowMgr);
	return S_OK;
}