#include "TopInclude.h"
#include "List.h"
#include "Memory.h"
#include "CoreComponent.h"
#include "Process.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "Singleton.h"
#include "EventManager.h"

#include "ProcessManager.h"

ID ProcessManager::addProcess(Process * pProcess) { 
	ID id;
	if (FAILED(mpProcesses->pushBack(pProcess, &id))) {
		EventManager::get()->queueEventND(EVENT_ADDPROCESS, pProcess);
	}
	return ; 
};

HRESULT ProcessManager::removeProcess(ID processID)
{
	if (mpProcesses) {
		return mpProcesses->deleteByID(processID);
	}
	return S_OK;
};

HRESULT ProcessManager::restore()
{
	if (!mpProcesses) { 
		mpProcesses = new List<Process>(); 
	}
	else {
		mpProcesses->restore();
	}
	return S_OK;
};

HRESULT ProcessManager::run(TIME elapsed)
{
	if (mpProcesses->getLength() == 0) { return S_OK; }



	ListElement<Process>* pListElem = mpProcesses->iterator();
	Process* pProcess;
	while (pListElem) {
		pProcess = pListElem->getObject();
		if (pProcess) {
			pProcess->run(elapsed);
		}
		if (pListElem) {
			pListElem = (ListElement<Process>*)pListElem->getNext();
		}
		if (mpProcesses->getLength() == 0) { return S_OK; }
	}
	return S_OK;
}
ProcessManager * ProcessManager::get()
{
	if (!mpInstance) {
		if (mAllowInstancing) {
			mpInstance = new ProcessManager();
			mpInstance->restoreCoreListeners();
		}
		else {
			CE1_ASSERT(0 && "Not allowed to instance Singleton");
		}
	}
	return mpInstance;
}