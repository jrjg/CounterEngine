#include "TopInclude.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "List.h"

#include "Memory.h"

MemoryManager::MemoryManager() : Singleton<MemoryManager>(false){
	mpList = new UnManagedList<MemManaged>();
}
MemoryManager::~MemoryManager() { 
	if (mpList) {
		if (mpList->getLength() > 0) {
			for (UnManagedListElement<MemManaged>* pElem = mpList->iterator(); pElem != NULL; pElem = pElem->getNext()) {
				if (!pElem->getDeleteContent()) {
					free((void*)pElem->getObject());
				}
			}
		}
		delete mpList;
	}
}

HRESULT MemoryManager::allocateMem(MemManaged ** ppMem, size_t size)
{
	*ppMem = (MemManaged*)malloc(size);
	MemManaged* pMem = *ppMem;
	ZeroMemory(pMem, size);
	pMem->mMemID = mpList->pushFront(pMem);
	return S_OK;
}
HRESULT MemoryManager::freeMem(MemManaged * pMem) { 
	return freeMem(pMem->getMemID());
}

HRESULT MemoryManager::allocateMem(void ** ppMem, size_t size, ID * pID)
{
	*ppMem = malloc(size);
	void* pMem = *ppMem;
	ZeroMemory(pMem, size);
	*pID = mpList->pushFront((MemManaged*)pMem);
	mpList->iterator()->setDeleteContent(false);
	return S_OK;
}

HRESULT MemoryManager::freeMem(ID id)
{
	free((void*)mpList->pop(id));
	return S_OK;
}
MemoryManager * MemoryManager::get()
{
	if (!mpInstance) {
		if (mAllowInstancing) {
			mpInstance = new MemoryManager();
		}
		else {
			CE1_ASSERT(0 && "Not allowed to instance Singleton");
			mpInstance->mpAllowInstancingListener = new AllowInstancingListener<MemoryManager>(mpInstance);
		}
	}
	return mpInstance;
};