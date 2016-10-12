#ifndef PROCESSHANDLER_INC
#define PROCESSHANDLER_INC

class ProcessHandler : public MemManaged {
protected:
	ID mProcessID;
public:
	void pause() { ProcessManager::get()->pauseProcess(mProcessID); };
	void unpause() { ProcessManager::get()->continueProcess(mProcessID); };
	virtual HRESULT handleProcess(TIME elapsed) = 0;
	ProcessHandler(TIME speed) : { mProcessID = ProcessManager::get()->newProcess(this, speed); };
	virtual ~ProcessHandler() { ProcessManager::get()->deleteProcess(mProcessID); };
};

#endif 