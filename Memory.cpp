#include "TopInclude.h"
#include "List.h"
#include "ListElement.h"
#include "SpecialList.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "Memory.h"

HRESULT MemoryManager::allocateMem(void ** ppMem, size_t size, ID* pMemID)
{
	*ppMem = malloc(size);
	void* pMem = *ppMem;
	ZeroMemory(pMem, size);
	*pMemID = mpList->pushFront(pMem);
	return S_OK;
};