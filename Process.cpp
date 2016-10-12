#include "TopInclude.h"
#include "Memory.h"
#include "ProcessManager.h"

#include "Process.h"

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
}