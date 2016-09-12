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
WindowMgr* WindowMgr_NEW();
BOOL WindowMgr_Run(TIME);
BOOL WindowMgr_DELETE(void);

#endif