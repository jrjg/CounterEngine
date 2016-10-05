#include "TopInclude.h"

#include "D3DBasic.h"

void * D3DBasic::operator new(size_t size)
{
	void* pObject =  malloc(size);
	ZeroMemory(pObject, size);
	return pObject;
}

void D3DBasic::operator~()
{
	free(this);
}
