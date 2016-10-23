#ifndef MAIN_INC
#define MAIN_INC

class RunningListener : public EventListener {
private:
	bool* mpIsRunning;
protected:
	HRESULT handle(MemManaged* pData) override { 
		*mpIsRunning = false; return S_OK; 
	};
public:
	RunningListener(bool* pIsRunning) : EventListener(EVENT_RELEASE), mpIsRunning(pIsRunning) {};
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif