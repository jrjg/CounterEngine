#include "TopInclude.h"
#include "Engine.h"
#include "EventManager.h"
#include "windowmgr.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
	WNDCLASSEX* pwc;
	_NEW(WNDCLASSEX, pwc);
	HRCALL(registerWindowClass(pwc, &(Engine_GetWinParams()->hInstance)));
	BOCALL(RegisterClassEx(pwc));
	(*pWindowMgr->phwnd) = CreateWindowEx(WS_EX_CLIENTEDGE, pwc->lpszClassName, pWindowMgr->windowtitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, BUFFERWIDTH, BUFFERHEIGHT, NULL, NULL, Engine_GetWinParams()->hInstance, NULL);
	POCHECK(*pWindowMgr->phwnd);
	return S_OK;
}

HRESULT showWindow(WindowMgr* pWindowMgr) {
	ShowWindow(*pWindowMgr->phwnd, Engine_GetWinParams()->nCmdShow);
	UpdateWindow(*pWindowMgr->phwnd);
	return S_OK;
}

WindowMgr* WindowMgr_NEW(void) {
	WindowMgr* pInst;
	_NEW(WindowMgr, pInst);
	_NEW(HWND, pInst->phwnd);
	pInst->windowtitle = WINDOWTITLE;
	SAFECALL(createWindow(pInst));
	SAFECALL(showWindow(pInst));
	return pInst;
}


BOOL WindowMgr_Run(TIME elapsed) {
	WindowMgr* pWindowMgr = Engine_GetWindowMgr();
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
	return ERROR_FAILURE;
}

BOOL WindowMgr_DELETE(void) {
	WindowMgr* pWindowMgr = Engine_GetWindowMgr();
	DestroyWindow(*(pWindowMgr->phwnd));
	_DEL(pWindowMgr->phwnd);
	_DEL(pWindowMgr);
	return ERROR_FAILURE;
}