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
	Vector* pVector;
	_NEW(Vector, pVector);
	if (!pVector)
	{
		return NULL;
	}
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
	return pVector->_last;
}

unsigned int Vector_Capacity(Vector* pVector)
{
	return pVector->_capacity;
}

BOOL Vector_Delete(Vector* pVector)
{
	_DEL(pVector->_pMem);
	_DEL(pVector);
	return ERROR_FAILURE;
}

BOOL Vector_DeleteAllElements(Vector* pVector)
{
	BOOL error = ERROR_FAILURE;
	if (pVector->_elems > 0)
	{
		for (unsigned int i = 0; i < pVector->_last; i++)
		{
			error = error || Vector_DeleteElement(pVector, i);
		}
	}
	return error;
}

BOOL Vector_FullDelete(Vector* pVector)
{
	BOOL error = ERROR_FAILURE;
	error = error || Vector_DeleteAllElements(pVector);
	error = error || Vector_Delete(pVector);
	return error;
}

BOOL Vector_DeleteElement(Vector* pVector, unsigned int index)
{
	_DEL(Vector_Get(pVector, index));
	pVector->_elems--;
	if (pVector->_last == index)
	{
		unsigned int i;
		for (i = index; i > 0 && Vector_Get(pVector, index) == NULL; i--)
		{
		}
		pVector->_last = i;
	}
	return ERROR_FAILURE;
}

BOOL Vector_Resize(Vector* pVector, unsigned int elemcnt)
{
	assert(pVector);
	void* pMem = malloc(elemcnt * pVector->_elemsize);
	memset(pMem, 0, elemcnt * pVector->_elemsize);
	if (pVector->_elems>0)
	{
		memcpy(pMem, pVector->_pMem, pVector->_capacity * pVector->_elemsize);
	}
	_DEL(pVector->_pMem);
	pVector->_pMem = pMem;
	pVector->_capacity = elemcnt;
	return ERROR_FAILURE;
}

BOOL Vector_Insert(Vector* pVector, unsigned int index, void* pData)
{
	BOOL error = ERROR_FAILURE;
	if(index > pVector->_capacity)
	{
		error = Vector_Resize(pVector, index * 2);
		if (error) { return error; };
	}
	memcpy((char*)(pVector->_pMem) + index * pVector->_elemsize, &pData, pVector->_elemsize);
	if (index > pVector->_last)
	{
		pVector->_last = index;
	}
	pVector->_elems++;
	return ERROR_FAILURE;
}

unsigned int Vector_Pushback(Vector* pVector, void* pData)
{
	BOOL error = ERROR_FAILURE;
	unsigned int index = Vector_Last(pVector);
	index++;
	if (index > pVector->_capacity)
	{
		error = Vector_Resize(pVector, index * 2);
		if (error) { return error; };
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
	return pVector->_elems;
}

void* Vector_Get(Vector* pVector, unsigned int index)
{
	if (index <= pVector->_capacity)
	{
		return(*(void**)((char*)(pVector->_pMem) + index * pVector->_elemsize));
	}
	return NULL;
}