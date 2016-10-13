#include "TopInclude.h"
#include "Memory.h"
#include "SimplyManaged.h"

#include "MemManaged.h"

void* MemManaged::operator new(size_t size) {
	MemManaged* pInst;
	MemoryManager::get()->allocateMem(&pInst, size);
	return pInst;
};
void MemManaged::operator delete(void* pInst) {
	if (!pInst) { return; }
	MemoryManager::get()->freeMem((MemManaged*)pInst);
	return;
};