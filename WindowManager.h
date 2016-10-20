#ifndef WINDOWMANAGER_INC
#define WINDOWMANAGER_INC

#include "Singleton.h"
#include "CoreComponent.h"
#include "EventListener.h"
#include "WinParams.h"
#include "Vector.h"

class WindowCreated : public MemManaged {
public:
	LPCWSTR mWNDTitle;
	UINT mID;
	WindowCreated(LPCWSTR WNDTitle,UINT ID) : mWNDTitle(WNDTitle), mID(ID){};
};

class WindowCreator : public MemManaged{
public:
	UINT mWNDWidth;
	UINT mWNDHeight;
	LPCWSTR mWNDTitle;
	WindowCreator(UINT WNDWidth, UINT WNDHeight, LPCWSTR WNDTitle) : mWNDWidth(WNDWidth), mWNDHeight(WNDHeight), mWNDTitle(WNDTitle) {};
};

class CreateWindowListener : public EventListener {
public:
	HRESULT handle(MemManaged* pData)override;
	virtual ~CreateWindowListener() {};
	CreateWindowListener() : EventListener(EVENT_CREATEWINDOW) {};
};

class WM_WinParamsListener : public EventListener {
public:
	HRESULT handle(MemManaged* pData)override;
	virtual ~WM_WinParamsListener() {};
	WM_WinParamsListener() : EventListener(EVENT_WINPARAMS) {};
};

class Window : public MemManaged {
private:
	LPCWSTR mWNDTitle;
	HWND mHWND;
public:
	LPCWSTR getWNDTitle() { return mWNDTitle; };
	void setWNDTitle(LPCWSTR WNDTitle) { mWNDTitle = WNDTitle; };
	HWND getHWND() { return mHWND; };
	HRESULT setHWND(HWND hwnd) { if (hwnd) { mHWND = hwnd; return S_OK; }; return ERROR_SUCCESS; };
	Window() {};
	virtual ~Window() { if (mHWND) { DestroyWindow(mHWND); }; };
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

class WindowManager : public CoreComponent, public Singleton<WindowManager> {
	friend class WM_WinParamsListener;
	friend class Singleton<WindowManager>;
private:
	WM_WinParamsListener* mpWinParamsListener;
	Vector<Window>* mpWindows;
	HRESULT evalWinParams(WinParams* pWinParams);
	HINSTANCE mHInstance;
	HINSTANCE mHPrevInstance;
	LPSTR mpCmdLine;
	int mCmdShow;
	CreateWindowListener* mpCreateWindowListener;
public:
	HRESULT createWindow(WindowCreator* pWindowCreator, UINT* pID);
	WindowManager() { restore(); };
	virtual ~WindowManager();
	HRESULT run(TIME elapsed)override;
	HRESULT restore()override;
};

#endif