#ifndef WINDOW_INC
#define WINDOW_INC


struct WindowMgr {
	LPCWSTR windowtitle;
	HWND* phwnd;
};

typedef struct WindowMgr WindowMgr;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT registerWindowClass(WNDCLASSEX*, HINSTANCE*);
HRESULT createWindow(WindowMgr*);
HRESULT showWindow(WindowMgr*);
HRESULT WindowMgr_NEW(WindowMgr**);
HRESULT WindowMgr_Run(TIME);
HRESULT WindowMgr_DELETE(void);

#endif