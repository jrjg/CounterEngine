#include "TopInclude.h"
#include "List.h"
#include "Vector.h"
#include "Timer.h"
#include "Engine.h"
#include "Memory.h"
#include "ListElement.h"

#include "ProcessManager.h"

ProcessManager* gpProcessManager;

ProcessManager::ProcessManager()
{
	mpProcesses = new List<Process>(true);
	mpIDs = new Vector<SimplyManaged<ID>>(true,300);
	mIDCounter = 0;
}

ID ProcessManager::newProcess(PCB pCallBack, TIME wait)
{
	Process* pNewProcess = new Process(pCallBack, wait, mIDCounter++);
	mpIDs->insert(new SimplyManaged<ID>(mpProcesses->pushBack(pNewProcess)), pNewProcess->getID());
	return pNewProcess->getID();
}

HRESULT ProcessManager::deleteProcess(ID id)
{
	mpProcesses->deleteByID(((SimplyManaged<ID>*)mpIDs->get(id))->mX);
	mpIDs->deleteByID(id);
	return S_OK;
}

HRESULT Process::run(TIME elapsed)
{
	mWaited = mWaited + elapsed;
	if (mWaited >= mWait)
	{
		if (mWait == 0)
		{
			(*mpCallBack)(mWait);
		}
		else
		{
			for (; mWaited >= mWait; mWaited = mWaited - mWait)
			{
				(*mpCallBack)(mWait);
			}
		}
	}
	return S_OK;
}

ProcessManager* ProcessManager::get() {
	if (!gpProcessManager) {
		gpProcessManager = new ProcessManager();
	}
	return gpProcessManager;
}

HRESULT ProcessManager::run(TIME elapsed)
{
	if (get()->mpProcesses->getLength() == 0) { return S_OK; }
	Process* pProcess;
	for (ListElement<Process>* pListElement = get()->mpProcesses->getFirst(); pListElement != NULL; pListElement = pListElement->getNext()) {
		pProcess = pListElement->getObject();
		if (pProcess->isRunning()) {
			pProcess->run(elapsed);
		}
	}
	return S_OK;
}