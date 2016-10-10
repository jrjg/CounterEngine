#ifndef INCLUDE_ENGINE
#define INCLUDE_ENGINE

class WinParams :public MemManaged {
public:
	WinParams(HINSTANCE HInstance, HINSTANCE HPrevInstance, LPSTR pCmdLine, int CmdShow) : mHInstance(HInstance),mHPrevInstance(HPrevInstance),mpCmdLine(pCmdLine), mCmdShow(CmdShow){};
	HINSTANCE mHInstance;
	HINSTANCE mHPrevInstance;
	LPSTR mpCmdLine;
	int mCmdShow;
};

//class EngineExtra : public MemManaged {
//public:
//	EngineExtra(LPCWSTR WindowTitle, float BufferWidth, float BufferHeight, bool Fullscreen, TIME TickDelay) : mWindowTitle(WindowTitle), mBufferWidth(BufferWidth), mBufferHeight(BufferHeight), mFullscreen(Fullscreen), mTickDelay(TickDelay) {};
//	LPCWSTR mWindowTitle;
//	float mBufferWidth;
//	float  mBufferHeight;
//	bool mFullscreen;
//	TIME mTickDelay;
//};

class Engine : public CoreComponent
{
private:
	EventManager* mpEventManager;
	Controller* mpController;
	ProcessManager* mpProcessManager;
	Timer* mpTimer;
	WindowManager* mpWindowManager;

	WinParams* mpWinParams;
	bool mRunning;
	TIME mTickSpeed;
	//EngineExtra* mpExtra;
	//HRESULT loadExtra(void*);
	//HRESULT readExtra(void* p0, String<char>* pObjName, void* pObj);
	/*~Engine() { delete mpExtra; delete mpWinParams; };*/
public:
	HRESULT restore() override;
	bool isRunning() { return mRunning; };
	HRESULT handleProcess(TIME elapsed)override;
	WinParams* getWinParams() { return mpWinParams; };
	EventManager* getEventManager() { return mpEventManager; };
	Controller* getController() { return mpController; };
	ProcessManager* getProcessManager() { return mpProcessManager; }
	Timer* getTimer() { return mpTimer; };
	WindowManager* getWindowManager() { return mpWindowManager; };
	Engine();
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

MemoryManager* getMemoryManager();

#endif