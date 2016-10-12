#include "TopInclude.h"
#include "List.h"
#include "Memory.h"
#include "ListElement.h"
#include "CoreComponent.h"
#include "Process.h"

#include "ProcessManager.h"

ProcessManager* ProcessManager::get() {
	if (!mpInstance) { 
		mpInstance = new ProcessManager(); 
		mpInstance->CoreComponent::restore();
	}; 
	return mpInstance; 
}

HRESULT ProcessManager::restore()
{
	delete mpProcesses; mpProcesses = new List<Process>();
	return S_OK;
}

HRESULT ProcessManager::run(TIME elapsed)
{
	if (mpProcesses->getLength() == 0) { return S_OK; }
	for (ListElement<Process>* pListElement = mpProcesses->getFirst(); pListElement != NULL; pListElement = pListElement->getNext()) {
		pListElement->getObject()->run(elapsed);
	}
	return S_OK;
}