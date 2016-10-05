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

class EngineExtra : public MemManaged {
public:
	EngineExtra(LPCWSTR WindowTitle, float BufferWidth, float BufferHeight, bool Fullscreen, TIME TickDelay) : mWindowTitle(WindowTitle), mBufferWidth(BufferWidth), mBufferHeight(BufferHeight), mFullscreen(Fullscreen), mTickDelay(TickDelay) {};
	LPCWSTR mWindowTitle;
	float mBufferWidth;
	float  mBufferHeight;
	bool mFullscreen;
	TIME mTickDelay;
};

class Engine : public MemManaged
{
private:
	WinParams* mpWinParams;
	bool mRunning;
	EngineExtra* mpExtra;
	Parser* mpConfigParser;
	Engine() { EventManager::registerForEvent(EVENT_RESTORE, &Engine::restore); };
	~Engine() {delete mpConfigParser;delete mpExtra;delete mpWinParams;};
	ID mProcess_Run;
	ID mProcess_WaitTimer;
public:
	static HRESULT restore(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	static HRESULT release() { delete gpEngine; };
	static Engine* get();
	static HRESULT run();

	static WinParams* getWinParams() { return get()->mpWinParams; };
	static HRESULT waitTimer(TIME elapsed) { Timer::wait(get()->mpExtra->mTickDelay - elapsed); };
	static HRESULT terminate(void* p0) { get()->mRunning = false; };
	static HRESULT loadExtra(void*);
	static HRESULT readExtra(void* p0, String<char>* pObjName, void* pObj);
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif