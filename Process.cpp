#include "TopInclude.h"
#include "Memory.h"
#include "ProcessManager.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "Process.h"

Process::Process(TIME shouldWait) : mShouldWait(shouldWait), mWaited(0), mRunning(true) { 
	ProcessManager* pProcessManager = ProcessManager::get();
	if (pProcessManager) {
		pProcessManager->addProcess(this,&mID);
	}
};

Process::~Process() { 
	ProcessManager* pPM = ProcessManager::get();
	if (pPM) { 
		pPM->removeProcess(mID); 
	};
};

HRESULT Process::run(TIME elapsed)
{
	if (!mRunning) { return S_OK; };
	mWaited += elapsed; 
	if (mWaited >= mShouldWait)
	{
		for (; mWaited >= mShouldWait; mWaited -= mShouldWait)
		{
			V_RETURN(handle(mShouldWait));
		}
	}
	return S_OK;
};