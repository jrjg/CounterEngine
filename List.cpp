#include "Globals.h"
#include "CLibrary.h"
#include "List.h"

ID List_ID = 0;

struct ListElement
{
	struct ListElement* _pNext;
	void* _pObj;
	ID _id;
};

struct List{
	ListElement* _pFirstElem;
	ListElement* _pLastElem;
	unsigned long _length;
	unsigned long _elemsize;
	ID _idcnt;
	ID _id;
};

List* List_New(unsigned long elemsize)
{
	assert(elemsize != 0);
	List* pNewList;
	_NEW(List, pNewList);
	if (!pNewList)
	{
		return NULL;
	}
	pNewList->_pFirstElem = NULL;
	pNewList->_pLastElem = NULL;
	pNewList->_elemsize = elemsize;
	pNewList->_length = 0;
	pNewList->_id = List_ID;
	pNewList->_idcnt = 0;
	List_ID++;
	return pNewList;
}

void* List_Get(ListElement* pElem)
{
	assert(pElem);
	return pElem->_pObj;
}

ListElement* List_Next(ListElement* pElem)
{
	assert(pElem);
	return pElem->_pNext;
}

Iterator List_Iterator(List* pList)
{
	assert(pList);
	return pList->_pFirstElem;
}

unsigned int List_Length(List* pList)
{
	assert(pList);
	return pList->_length;
}

void* List_Pop(List* pList)
{
	assert(pList);
	if (pList->_length == 0)
	{
		return NULL;
	}
	ListElement* pFirst = pList->_pFirstElem;
	void* pObj = pFirst->_pObj;
	pList->_pFirstElem = pFirst->_pNext;
	_DEL(pFirst);
	pList->_length--;
	if (pList->_length == 0)
	{
		pList->_pFirstElem = NULL;
		pList->_pLastElem = NULL;
	}
	return pObj;
}

BOOL List_Delete(List* pList)
{	
	_DEL(pList);
	return ERROR_FAILURE;
}

BOOL List_FullDelete(List* pList)
{
	if (!pList)
	{
		return ERROR_SUCCESS;
	}
	BOOL error = ERROR_FAILURE;
	error = error || List_DeleteAllElements(pList);
	error = error || List_Delete(pList);
	return error;
}

BOOL List_DeleteAllElements(List* pList)
{
	BOOL error = ERROR_FAILURE;
	Iterator itr2 = NULL;
	for (Iterator itr = List_Iterator(pList); itr != NULL; itr = List_Next(itr)) {
		if (itr2)
		{
			error = error || List_DeleteElement(pList, itr2->_id);
		}
		itr2 = itr;
	}
	return error;
}

BOOL List_DeleteElement(List* pList, ID id)
{
	Iterator itr;
	Iterator itr2;
	if (pList->_pFirstElem->_id == id) {
		itr = pList->_pFirstElem;
		pList->_pFirstElem = pList->_pFirstElem->_pNext;
		_DEL(itr->_pObj);
		_DEL(itr);
		pList->_length--;
		if (pList->_length == 0)
		{
			pList->_pFirstElem = NULL;
			pList->_pLastElem = NULL;
		}
		return ERROR_FAILURE;
	} else {
		for (itr = List_Iterator(pList); List_Next(itr) != NULL; itr = List_Next(itr)) {
			if (itr->_id == id){
				_DEL(itr->_pObj);
				assert(itr2);
				itr2->_pNext = itr->_pNext;
				if (pList->_pLastElem = itr)
				{
					pList->_pLastElem = itr2;
				}
				_DEL(itr);
				pList->_length--;
				return ERROR_FAILURE;
			}
			itr2 = itr;
		}
	}
	return ERROR_SUCCESS;
}

ID List_PushBack(List* pList, void* pObject)
{
	if (!pList)
	{
	}
	ListElement* pNewElem;
	_NEW(ListElement, pNewElem);
	if (!pNewElem)
	{
		return 0;
	}
	pNewElem->_pNext = NULL; 
	pNewElem->_pObj = pObject;
	pList->_idcnt++;
	pNewElem->_id = pList->_idcnt;
	if (pList->_length == 0)
	{
		pList->_pFirstElem = pNewElem;
		pList->_pLastElem = pNewElem;
	}
	else {
		assert(pList->_pLastElem);
		pList->_pLastElem->_pNext = pNewElem;
		pList->_pLastElem = pNewElem;
	}
	pList->_length++;
	return pNewElem->_id;
}