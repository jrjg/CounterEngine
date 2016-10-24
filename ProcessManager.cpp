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

HRESULT ProcessManager::addProcess(Process * pProcess,ID* pID) {
	HRESULT hr = mpProcesses->pushBack(pProcess, pID);
	if (FAILED(hr)) {
		EventManager* pEventManager = EventManager::get();
		if (pEventManager) {
			hr = pEventManager->queueEventND(EVENT_ADDPROCESS, pProcess);
		}
	}
	return hr; 
};

HRESULT ProcessManager::removeProcess(ID processID)
{
	if(FAILED(mpProcesses->deleteByID(processID))){
		EventManager* pEventManager = EventManager::get();
		if (pEventManager) {
			SimplyManaged<ID>* pID = new SimplyManaged<ID>(processID);
			if (FAILED(pEventManager->queueEventND(EVENT_REMOVEPROCESS, pID))) {
				delete pID;
				return ERROR_SUCCESS;
			}
		}
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
	ID processesKey = 0;
	V_RETURN(mpProcesses->lock(&processesKey));
	ListElement<Process>* pListElem = mpProcesses->iterator();
	Process* pProcess = 0;
	while (pListElem) {
		pProcess = pListElem->getObject();
		if (pProcess) {
			pProcess->run(elapsed);
		}
		pListElem = (ListElement<Process>*)pListElem->getNext();
	}
	V_RETURN(mpProcesses->unlock(processesKey));
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