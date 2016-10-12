#ifndef PROCESS_INC
#define PROCESS_INC

class Process : public MemManaged
{
private:
	TIME mShouldWait;
	TIME mWaited;
	bool mRunning;
	ID mID;
protected:
	virtual HRESULT handle(TIME elapsed) = 0;
public:
	Process(TIME shouldWait) : mShouldWait(shouldWait), mWaited(0), mRunning(true) { mID = ProcessManager::get()->addProcess(this); };
	~Process() { ProcessManager::get()->removeProcess(mID); };
	void pause() { mRunning = false; };
	void unPause() { mRunning = true; };
	HRESULT run(TIME elapsed);
};

#endif