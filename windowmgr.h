//#ifndef WINDOW_INC
//#define WINDOW_INC
//
//class WindowManager : public MemManaged {
//private:
//	LPCWSTR mWNDTitle;
//	HWND mHWND;
//	int mWNDWidth;
//	int mWNDHeight;
//	HINSTANCE mHINSTANCE;
//	int mCMDShow;
//	HRESULT createWindow();
//	WindowManager();
//	~WindowManager() { DestroyWindow(mHWND); };
//public:
//	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//	static HRESULT run(TIME);
//	static WindowManager* get();
//	static HRESULT restore(void* p0);
//	static HRESULT release(void* p0) { delete get(); };
//};
//
//#endif