#include "TopInclude.h"
#include "Memory.h"
#include "SimplyManaged.h"

#include "MemManaged.h"

void * MemManaged::operator new(size_t size)
{
	void * pInst;
	MemoryManager::get()->allocateMem((MemManaged**)&pInst, size);
	return pInst;
}

void MemManaged::operator delete(void * pInst)
{
	if (pInst) { 
		MemoryManager::get()->freeMem((MemManaged*)pInst);
	};
}
