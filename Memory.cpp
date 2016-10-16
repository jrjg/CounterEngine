#include "TopInclude.h"
#include "ListElement.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "UnManagedList.h"

#include "Memory.h"

MemoryManager::MemoryManager() : Singleton<MemoryManager>(false){
	mpList = new UnManagedList<MemManaged>();
}
MemoryManager::~MemoryManager() { 
	if (mpList->getLength() > 0) {
		for (UnManagedListElement<MemManaged>* pElem = mpList->getFirst(); pElem != NULL; pElem = pElem->getNext()) {
			if (!pElem->getDeleteContent()) {
				free((void*)pElem->getObject());
			}
		}
	}
	SAFE_RELEASE(mpList);
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
	if (pMem) {
		for (UnManagedListElement<MemManaged>* pElem = mpList->getFirst(); pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == pMem->getMemID()) {
				pElem->setDeleteContent(false);
				mpList->deleteListElement(pElem);
				break;
			}
		}
		free(pMem);
	};
	return S_OK; 
}

HRESULT MemoryManager::allocateMem(void ** ppMem, size_t size, ID * pID)
{
	*ppMem = malloc(size);
	void* pMem = *ppMem;
	ZeroMemory(pMem, size);
	*pID = mpList->pushFront((MemManaged*)pMem);
	mpList->getFirst()->setDeleteContent(false);
	return S_OK;
}

HRESULT MemoryManager::freeMem(ID id)
{
	free((void*)mpList->pop(id));
	return S_OK;
};