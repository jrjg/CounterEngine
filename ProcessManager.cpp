#include "TopInclude.h"
#include "List.h"
#include "Memory.h"
#include "ListElement.h"
#include "CoreComponent.h"
#include "Process.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "ProcessManager.h"

ProcessManager* gpProcessManager;

ProcessManager::ProcessManager() : CoreComponent(false)
{
	mpProcesses = new List<Process>(); 
}

ProcessManager* ProcessManager::get() {
	if (!gpProcessManager) {
		gpProcessManager = new ProcessManager();
		gpProcessManager->CoreComponent::restore();
	}; 
	return gpProcessManager;
};

HRESULT ProcessManager::restore()
{
	if (!mpProcesses) { mpProcesses = new List<Process>(); }; mpProcesses->restore();
	return S_OK;
};

HRESULT ProcessManager::run(TIME elapsed)
{
	if (mpProcesses->getLength() == 0) { return S_OK; }
	for (ListElement<Process>* pListElement = mpProcesses->getFirst(); pListElement != NULL; pListElement = pListElement->getNext()) {
		pListElement->getObject()->run(elapsed);
	}
	return S_OK;
};