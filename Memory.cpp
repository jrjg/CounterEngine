#include "TopInclude.h"
#include "ListElement.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "UnManagedList.h"

#include "Memory.h"

MemoryManager* gpMemoryManager;

MemoryManager::MemoryManager() { 
	mpList = new UnManagedList<MemManaged>();
}
MemoryManager::~MemoryManager() { mpList->release(); }
MemoryManager * MemoryManager::get()
{
	if (!gpMemoryManager) {
		gpMemoryManager = new MemoryManager();
	}; 
	return gpMemoryManager;
};

HRESULT MemoryManager::allocateMem(MemManaged ** ppMem, size_t size)
{
	*ppMem = (MemManaged*)malloc(size);
	MemManaged* pMem = *ppMem;
	ZeroMemory(pMem, size);
	pMem->mMemID = mpList->pushFront(pMem);
	return S_OK;
}
HRESULT MemoryManager::freeMem(MemManaged * pMem) { mpList->deleteByID(pMem->mMemID); return S_OK; };