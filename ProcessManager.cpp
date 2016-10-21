#include "TopInclude.h"
#include "List.h"
#include "Memory.h"
#include "CoreComponent.h"
#include "Process.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "Singleton.h"

#include "ProcessManager.h"

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
	for (ListElement<Process>* pListElement = (ListElement<Process>*)mpProcesses->iterator(); pListElement != NULL; pListElement = (ListElement<Process>*)pListElement->getNext()) {
		pListElement->getObject()->run(elapsed);
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