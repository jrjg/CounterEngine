#include "CLibrary.h"
#include "Globals.h"
#include "Vector.h"

ID Vector_ID = 0;

struct Vector
{
	void* _pMem;
	unsigned int _capacity; 
	unsigned int _last;
	unsigned int _elemsize;
	unsigned int _elems;
	ID _id;
};

Vector* Vector_New(unsigned long elemsize, unsigned int elemcnt)
{
	CE1_ASSERT(elemsize&&elemcnt);
	Vector* pVector;
	_NEW(Vector, pVector);
	pVector->_elemsize = elemsize;
	pVector->_capacity = 0;
	pVector->_last = 0;
	pVector->_pMem = NULL;
	pVector->_elems = 0;
	pVector->_id = Vector_ID;
	Vector_ID++;
	Vector_Resize(pVector, elemcnt);
	return pVector;
}

HRESULT Vector_GetMem(Vector* pVector, void** ppMem) {
	*ppMem = pVector->_pMem;
	return S_OK;
}

HRESULT Vector_Erase(Vector * pVector, UINT index, bool deletecontent)
{
	CE1_ASSERT(pVector&&"Vector_Erase");
	if (pVector->_elems > 0) {
		CE1_CALL(Vector_DeleteElement(pVector, index, deletecontent));
		CE1_CALL(Vector_Insert(pVector,index, Vector_Get(pVector,Vector_Last(pVector))));
		CE1_CALL(Vector_DeleteElement(pVector, Vector_Last(pVector), false));
	}
	return S_OK;
}

unsigned int Vector_Last(Vector* pVector)
{
	CE1_ASSERT(pVector);
	return pVector->_last;
}

unsigned int Vector_Capacity(Vector* pVector)
{
	CE1_ASSERT(pVector);
	return pVector->_capacity;
}

HRESULT Vector_Delete(Vector* pVector)
{
	if (!pVector) {
		return S_OK;
	}
	CE1_DEL(pVector->_pMem);
	CE1_DEL(pVector);
	return S_OK;
}

HRESULT Vector_DeleteAllElements(Vector* pVector, bool deletecontent)
{
	if (!pVector) {
		return S_OK;
	}
	if (pVector->_elems > 0)
	{
		for (unsigned int i = 0; i < pVector->_last; i++)
		{
			CE1_CALL(Vector_DeleteElement(pVector, i, deletecontent));
		}
	}
	return S_OK;
}

HRESULT Vector_FullDelete(Vector* pVector, bool deletecontent)
{
	if (!pVector) {
		return S_OK;
	}
	CE1_CALL(Vector_DeleteAllElements(pVector, deletecontent));
	CE1_CALL(Vector_Delete(pVector));
	return S_OK;
}

HRESULT Vector_DeleteElement(Vector* pVector, unsigned int index,bool deletecontent)
{
	if (!pVector) {
		return S_OK;
	}
	if (deletecontent) { CE1_DEL(Vector_Get(pVector, index)) };
	pVector->_elems--;
	ZeroMemory((char*)(pVector->_pMem) + index * pVector->_elemsize, pVector->_elemsize);
	if (pVector->_last == index)
	{
		unsigned int i;
		for (i = index; i > 0 && Vector_Get(pVector, index) == NULL; i--)
		{
		}
		pVector->_last = i;
	}
	return S_OK;
}

HRESULT Vector_Resize(Vector* pVector, unsigned int elemcnt)
{
	CE1_ASSERT(pVector&&elemcnt);
	void* pMem = malloc(elemcnt * pVector->_elemsize);
	memset(pMem, 0, elemcnt * pVector->_elemsize);
	if (pVector->_elems>0)
	{
		memcpy(pMem, pVector->_pMem, pVector->_capacity * pVector->_elemsize);
	}
	CE1_DEL(pVector->_pMem);
	pVector->_pMem = pMem;
	pVector->_capacity = elemcnt;
	return S_OK;
}

HRESULT Vector_Insert(Vector* pVector, unsigned int index, void* pData)
{
	CE1_ASSERT(pVector&&pData);
	if(index > pVector->_capacity)
	{
		CE1_CALL(Vector_Resize(pVector, index * 2));
	}
	memcpy((char*)(pVector->_pMem) + index * pVector->_elemsize, &pData, pVector->_elemsize);
	if (index > pVector->_last)
	{
		pVector->_last = index;
	}
	pVector->_elems++;
	return S_OK;
}

unsigned int Vector_Pushback(Vector* pVector, void* pData)
{
	CE1_ASSERT(pVector&&pData);
	unsigned int index = Vector_Last(pVector);
	index++;
	if (index > pVector->_capacity)
	{
		CE1_CALL(Vector_Resize(pVector, index * 2));
	}
	memcpy((char*)(pVector->_pMem) + index * pVector->_elemsize, &pData, pVector->_elemsize);
	if (index > pVector->_last)
	{
		pVector->_last = index;
	}
	pVector->_elems++;
	return index;
}

unsigned int Vector_Elements(Vector* pVector)
{
	CE1_ASSERT(pVector);
	return pVector->_elems;
}

void* Vector_Get(Vector* pVector, unsigned int index)
{
	CE1_ASSERT(pVector);
	if (index <= pVector->_capacity)
	{
		return(*(void**)((char*)(pVector->_pMem) + index * pVector->_elemsize));
	}
	return NULL;
}