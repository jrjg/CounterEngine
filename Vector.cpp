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
	CEASSERT(elemsize&&elemcnt);
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

unsigned int Vector_Last(Vector* pVector)
{
	CEASSERT(pVector);
	return pVector->_last;
}

unsigned int Vector_Capacity(Vector* pVector)
{
	CEASSERT(pVector);
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

HRESULT Vector_DeleteAllElements(Vector* pVector)
{
	if (!pVector) {
		return S_OK;
	}
	if (pVector->_elems > 0)
	{
		for (unsigned int i = 0; i < pVector->_last; i++)
		{
			CE1_CALL(Vector_DeleteElement(pVector, i));
		}
	}
	return S_OK;
}

HRESULT Vector_FullDelete(Vector* pVector)
{
	if (!pVector) {
		return S_OK;
	}
	CE1_CALL(Vector_DeleteAllElements(pVector));
	CE1_CALL(Vector_Delete(pVector));
	return S_OK;
}

HRESULT Vector_DeleteElement(Vector* pVector, unsigned int index)
{
	if (!pVector) {
		return S_OK;
	}
	CE1_DEL(Vector_Get(pVector, index));
	pVector->_elems--;
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
	CEASSERT(pVector&&elemcnt);
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
	CEASSERT(pVector&&pData);
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
	CEASSERT(pVector&&pData);
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
	CEASSERT(pVector);
	return pVector->_elems;
}

void* Vector_Get(Vector* pVector, unsigned int index)
{
	CEASSERT(pVector);
	if (index <= pVector->_capacity)
	{
		return(*(void**)((char*)(pVector->_pMem) + index * pVector->_elemsize));
	}
	return NULL;
}