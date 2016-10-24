#include "TopInclude.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "List.h"

#include "Memory.h"

MemoryManager::MemoryManager() : Singleton<MemoryManager>(false){
	mpList = new UnManagedList<MemManaged>();
}

MemoryManager::~MemoryManager() { 
	ID memoryKey;
	if (SUCCEEDED(mpList->lock(&memoryKey))) {
		UnManagedListElement<MemManaged>* pElem = mpList->iterator();
		while (pElem) {
			if (!pElem->getDeleteContent()) {
				free((void*)pElem->getObject());
			}
			pElem = (UnManagedListElement<MemManaged>*)pElem->getNext();
		}
		mpList->unlock(memoryKey);
	}
	SAFE_RELEASE(mpList);
}

HRESULT MemoryManager::allocateMem(MemManaged ** ppMem, size_t size)
{
	V_RETURN(mpList->unlock(0));
	*ppMem = (MemManaged*)malloc(size);
	MemManaged* pMem = *ppMem;
	ZeroMemory(pMem, size);
	return mpList->pushFront(pMem, &pMem->mMemID);
}
HRESULT MemoryManager::freeMem(MemManaged * pMem) { 
	return freeMem(pMem->getMemID());
}

HRESULT MemoryManager::allocateMem(void ** ppMem, size_t size, ID * pID)
{
	V_RETURN(mpList->unlock(0));
	*ppMem = malloc(size);
	void* pMem = *ppMem;
	ZeroMemory(pMem, size);
	HRESULT hr = mpList->pushFront((MemManaged*)pMem, pID);
	if (SUCCEEDED(hr)) {
		UnManagedListElement<MemManaged>* pListElem = mpList->iterator();
		if (pListElem) {
			hr = pListElem->setDeleteContent(false);
		}
	}
	return hr;
}

HRESULT MemoryManager::freeMem(ID id)
{
	void* pObject;
	if (SUCCEEDED(mpList->pop(id, (MemManaged**)&pObject))) {
		free(pObject);
	}
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