#include "TopInclude.h"
#include "Memory.h"
#include "SimplyManaged.h"

#include "MemManaged.h"

void * MemManaged::operator new(size_t size)
{
	void * pInst = 0;
	MemoryManager* pMemoryManager = MemoryManager::get();
	if (pMemoryManager) {
		pMemoryManager->allocateMem((MemManaged**)&pInst, size);
	}
	return pInst;
}

void MemManaged::operator delete(void * pInst)
{
	MemoryManager* pMM = MemoryManager::get();
	if (pMM) {
		pMM->freeMem((MemManaged*)pInst);
	}
}

MemManaged::MemManaged()
{
	MemoryManager* pMM = MemoryManager::get();
	if (pMM) {
		mMemID = pMM->retrieveMyMemID();
	}
}
