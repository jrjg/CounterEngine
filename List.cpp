#include "Globals.h"
#include "CLibrary.h"
#include "List.h"

ID List_ID = 0;

HRESULT List_GetLast(List* pList,void** ppObject) {
	CEASSERT(pList&&List_Length(pList)>0 &&"invalid List");
	(*ppObject) = List_Get(pList->_pLastElem);
	return S_OK;
}

HRESULT List_Copy(List* pFrom, List* pTo) {
	CEASSERT(pFrom && pTo&&"invalid List"&&"List_Copy");
	ExecOnList(
		pFrom,
		List_PushBack(pTo, List_Get(itr));
	);
	return S_OK;
}

List* List_New(unsigned long elemsize)
{
	CEASSERT(elemsize != 0 && elemsize&&"list cannot hold elements of size 0"&&"List_New");
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
	CEASSERT(pElem&&"List_Get");
	return pElem->_pObj;
}

ListElement* List_Next(ListElement* pElem)
{
	CEASSERT(pElem&&"List_Next");
	return pElem->_pNext;
}

Iterator List_Iterator(List* pList)
{
	CEASSERT(pList&&"List_Iterator");
	return pList->_pFirstElem;
}

unsigned int List_Length(List* pList)
{
	CEASSERT(pList&&"List_Length");
	return pList->_length;
}

void* List_Pop(List* pList)
{
	CEASSERT(pList&&"List_Pop");
	if (pList->_length == 0)
	{
		return NULL;
	}
	ListElement* pFirst = pList->_pFirstElem;
	void* pObj = pFirst->_pObj;
	pList->_pFirstElem = pFirst->_pNext;
	CE1_DEL(pFirst);
	pList->_length--;
	if (pList->_length == 0)
	{
		pList->_pFirstElem = NULL;
		pList->_pLastElem = NULL;
	}
	return pObj;
}

HRESULT List_FullDelete(List* pList, BOOL deleteObject)
{
	if (!pList) {
		return S_OK;
	}
	CE1_CALL(ListCE1_DELeteAllElements(pList, deleteObject));
	CE1_DEL(pList);
	return S_OK;
}

HRESULT List_DeleteAllElements(List* pList, BOOL deleteObject)
{
	if (!pList) {
		return S_OK;
	}
	Iterator itr2 = NULL;
	for (Iterator itr = List_Iterator(pList); itr != NULL; itr = List_Next(itr)) {
		if (itr2)
		{
			CE1_CALL(ListCE1_DELeteElement(pList, itr2->_id, deleteObject));
		}
		itr2 = itr;
	}
	return S_OK;
}

HRESULT List_DeleteElement(List* pList, ID id,BOOL deleteObject)
{
	if (!pList) {
		return S_OK;
	}
	Iterator itr;
	Iterator itr2;
	if (pList->_pFirstElem->_id == id) {
		itr = pList->_pFirstElem;
		pList->_pFirstElem = pList->_pFirstElem->_pNext;
		if (deleteObject) {
			CE1_DEL(itr->_pObj);
		}
		CE1_DEL(itr);
		pList->_length--;
		if (pList->_length == 0)
		{
			pList->_pFirstElem = NULL;
			pList->_pLastElem = NULL;
		}
		return S_OK;
	} else {
		for (itr = List_Iterator(pList); List_Next(itr) != NULL; itr = List_Next(itr)) {
			if (itr->_id == id){
				if (deleteObject) {
					CE1_DEL(itr->_pObj);
				}
				assert(itr2);
				itr2->_pNext = itr->_pNext;
				if (pList->_pLastElem = itr)
				{
					pList->_pLastElem = itr2;
				}
				CE1_DEL(itr);
				pList->_length--;
				return S_OK;
			}
			itr2 = itr;
		}
	}
	return S_OK;
}

ID List_PushBack(List* pList, void* pObject)
{
	CEASSERT(pList&&pObject&&"List_PushBack");
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