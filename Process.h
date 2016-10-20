#ifndef PROCESS_INC
#define PROCESS_INC

#include "MemManaged.h"

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
	Process(TIME shouldWait);
	void pause() { mRunning = false; };
	void unPause() { mRunning = true; };
	HRESULT run(TIME elapsed);
	virtual ~Process();
};

#endif