#include "TopInclude.h"
#include "List.h"
#include "ListElement.h"

#include "Memory.h"

MemoryManager* gpMemoryManager;

inline void * MemManaged::operator new(size_t size)
{
	void* pInst;
	HRESULT hr;
	hr = MemoryManager::get()->allocateMem(&pInst, size, &((MemManaged*)pInst)->mMemID);
	CE1_ASSERT(SUCCEEDED(hr)&&"Memory allocation failed");
	return pInst;
}

inline void MemManaged::operator delete(void * pInst)
{
	if (!pInst) { return; }
	HRESULT hr;
	hr = MemoryManager::get()->freeMem(pInst);
	CE1_ASSERT(SUCCEEDED(hr) && "Memory freeing failed");
	return;
}

HRESULT MemoryManager::allocateMem(void ** ppMem, size_t size, ID* pMemID)
{
	*ppMem = malloc(size);
	void* pMem = *ppMem;
	ZeroMemory(pMem, size);
	*pMemID = mpList->pushFront(pMem);
	return S_OK;
}

MemoryManager * MemoryManager::get() {
	if (!gpMemoryManager) {
		gpMemoryManager = new MemoryManager();
	}
	return gpMemoryManager;
}