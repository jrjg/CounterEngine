#include "TopInclude.h"
#include "Memory.h"
#include "ProcessManager.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "Process.h"

Process::Process(TIME shouldWait) : mShouldWait(shouldWait), mWaited(0), mRunning(true) { mID = ProcessManager::get()->addProcess(this); };

Process::~Process() { 
	ProcessManager* pPM = ProcessManager::get();
	if (pPM) { pPM->removeProcess(mID); };
};

HRESULT Process::run(TIME elapsed)
{
	if (!mRunning) { return S_OK; };
	mWaited += elapsed; 
	if (mWaited >= mShouldWait)
	{
		for (; mWaited >= mShouldWait; mWaited -= mShouldWait)
		{
			handle(mShouldWait);
		}
	}
	return S_OK;
};