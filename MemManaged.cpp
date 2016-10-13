#include "TopInclude.h"
#include "Memory.h"
#include "SimplyManaged.h"

#include "MemManaged.h"

inline void * MemManaged::operator new(size_t size)
{
	void* pInst;
	MemoryManager::get()->allocateMem(&pInst, size, &((MemManaged*)pInst)->mMemID);
	return pInst;
}

inline void MemManaged::operator delete(void * pInst)
{
	if (!pInst) { return; }
	MemoryManager::get()->freeMem(pInst);
	return;
}
