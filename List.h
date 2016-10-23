#ifndef INCLUDE_LIST
#define INCLUDE_LIST

#include "TopInclude.h"
#include "MemManaged.h"
#include "Lockable.h"

template <class ObjectType>
class UnManagedListElement
{
	friend class ListElement<ObjectType>;
	friend class List<ObjectType>;
	friend class UnManagedList<ObjectType>;
protected:
	void* mpPrevious;
	void* mpNext;
	ObjectType* mpObject;
	ID mID;
	bool mDeleteContent;
	HRESULT setNext(void* p) { V_RETURN(access()); mpNext = p; return S_OK; };
	HRESULT setPrevious(void* p) { V_RETURN(access()); mpPrevious = p; return S_OK; };
	HRESULT setDeleteContent(bool b) { V_RETURN(access()); mDeleteContent = b; return S_OK; };
	UnManagedListElement(ObjectType* pObj, ID id, void* pNext, void* pPrevious, bool memManageContent) : mpObject(pObj), mID(id), mDeleteContent(memManageContent), mpNext(pNext), mpPrevious(pPrevious) {};
	virtual ~UnManagedListElement() { if (mDeleteContent) { SAFE_RELEASE(mpObject); }; };
public:
	HRESULT getID(ID* pID) { *pID = mID; return S_OK; };
	HRESULT getNext(void** ppNext) { *ppNext = mpNext; return S_OK; };
	HRESULT getPrevious(void** ppPrevious) { *ppPrevious = mpPrevious; return S_OK; };
	HRESULT getDeleteContent(bool* pB) { *pB = mDeleteContent; return S_OK; };
	HRESULT getObject(ObjectType** ppObject) { *ppObject = mpObject; return S_OK; };
	HRESULT Release() { V_RETURN(access()); delete this; return S_OK; };
};

template <class ObjectType>
class ListElement : public UnManagedListElement<ObjectType>, public MemManaged {
protected:
	virtual ~ListElement() {};
	ListElement(ObjectType* pObj, ID id, UnManagedListElement<ObjectType>* pNext, UnManagedListElement<ObjectType>* pPrevious, bool memManageContent) : UnManagedListElement<ObjectType>(pObj, id, pNext, pPrevious, memManageContent) {};
	HRESULT Release()override { V_RETURN(access()); delete this; return S_OK; };
};

template <class ObjectType, class ListElemType>
class List_Template : public Lockable {
protected:
	ListElemType* getLast() { return mpLastElem; };
	ListElemType* mpFirstElem;
	ListElemType* mpLastElem;
	UINT mLength;
	ID mElemIDCounter;
	bool mManageContent;
	virtual ~List_Template() { deleteAllListElements(); };
	HRESULT deleteAllListElements();
public:
	HRESULT deleteListElement(ListElemType* pListElem);
	HRESULT pushBack(ObjectType* pObj, ID* pID);
	HRESULT pushFront(ObjectType* pObj, ID* pID);
	HRESULT setLength(unsigned int length) { V_RETURN(access()); mLength = length; return S_OK; };
	HRESULT setFirst(ListElemType* pElem) { V_RETURN(access()); mpFirstElem = pElem; return S_OK; };
	HRESULT setLast(ListElemType* pElem) { V_RETURN(access()); mpLastElem = pElem; return S_OK; };
	HRESULT popFirst(ObjectType** ppObject) { V_RETURN(access()); pop(mpFirstElem, ppObject); return S_OK; };
	HRESULT popLast(ObjectType** ppObject) { V_RETURN(access()); pop(mpLastElem, ppObject); return S_OK; };
	HRESULT pop(ID id, ObjectType** ppObject);
	HRESULT pop(ListElemType* pElem, ObjectType** ppObject);
	HRESULT deleteByID(ID id);
	HRESULT restore();
	HRESULT Release() { V_RETURN(access()); delete this; return S_OK; };
	
	UINT getLength( {return mLength; };
	ListElemType* iterator() { return mpFirstElem; };
	ObjectType* getByID(ID id);
	List_Template() : mpFirstElem(NULL), mpLastElem(NULL), mLength(0), mElemIDCounter(0), mManageContent(true), mLocked(false) { restore(); };
};

template<class ObjectType,class ListElemType>
inline HRESULT List_Template<ObjectType, ListElemType>::restore()
{
	V_RETURN(access());
	deleteAllListElements();
	mpFirstElem = NULL;
	mpLastElem = NULL;
	mLength = 0;
	mElemIDCounter = 0;
	mManageContent = true;
	return S_OK;
};

template<class ObjectType,class ListElemType>
inline HRESULT List_Template<ObjectType, ListElemType>::deleteListElement(ListElemType* pListElem)
{
	V_RETURN(access());
	if (pListElem) {
		if (mpFirstElem->getID() == pListElem->getID()) { mpFirstElem = (ListElemType*)pListElem->getNext(); };
		if (mpLastElem->getID() == pListElem->getID()) { mpLastElem = (ListElemType*)pListElem->getPrevious(); };
		if (pListElem->getNext()) { ((ListElemType*)pListElem->getNext())->setPrevious((ListElemType*)pListElem->getPrevious()); };
		if (pListElem->getPrevious()) { ((ListElemType*)pListElem->getPrevious())->setNext((ListElemType*)pListElem->getNext()); };
		mLength--;
		delete pListElem;
	}
	return S_OK;
};

template <class ObjectType,class ListElemType>
ObjectType* List_Template<ObjectType, ListElemType>::getByID(ID id) {
	if (mLength > 0) {
		for (ListElemType* pElem = (ListElemType*)mpFirstElem; pElem != NULL; pElem = (ListElemType*)pElem->getNext()) {
			if (pElem->getID() == id) {
				return pElem->getObject();
			}
		}
	}
	return NULL;
}
template<class ObjectType, class ListElemType>
inline HRESULT List_Template<ObjectType, ListElemType>::deleteByID(ID id)
{
	V_RETURN(access());
	if (mLength > 0) {
		for (ListElemType* pElem = mpFirstElem; pElem != NULL; pElem = (ListElemType*)pElem->getNext()) {
			if (pElem->getID() == id) {
				return deleteListElement(pElem);
			}
		}
	}
	return S_OK;
};

template<class ObjectType,class ListElemType>
inline HRESULT List_Template<ObjectType, ListElemType>::pop(ID id, ObjectType ** ppObject)
{
	V_RETURN(access());
	if (mLength > 0) {
		for (ListElemType* pElem = mpFirstElem; pElem != NULL; pElem = (ListElemType*)pElem->getNext()) {
			if (pElem->getID() == id) {
				return pop(pElem, ppObject);
			}
		}
	}
	return ERROR_SUCCESS;
}

template<class ObjectType,class ListElemType>
inline HRESULT List_Template<ObjectType, ListElemType>::pop(ListElemType* pElem, ObjectType ** ppObject)
{
	V_RETURN(access());
	ObjectType* pObj;
	if (pElem) {
		*ppObject = pElem->getObject();
		pElem->setDeleteContent(false); //dont delete what is returned
		deleteListElement(pElem);
		return S_OK;
	}
	return ERROR_SUCCESS;
};

template <class ObjectType,class ListElemType>
HRESULT List_Template<ObjectType, ListElemType>::pushBack(ObjectType* pObject, ID* pID)
{
	V_RETURN(access());
	ListElemType* pNewElem = new ListElemType(pObject, mElemIDCounter++, NULL, mpLastElem, mManageContent);
	if (mpLastElem) {
		mpLastElem->setNext(pNewElem);
	};
	mpLastElem = pNewElem;
	if (!mpFirstElem) { mpFirstElem = pNewElem; };
	mLength++;
	*pID = pNewElem->getID();
	return S_OK;
};

template <class ObjectType,class ListElemType>
HRESULT List_Template<ObjectType, ListElemType>::pushFront(ObjectType* pObject, ID* pID)
{
	V_RETURN(access());
	ListElemType* pNewElem = new ListElemType(pObject, mElemIDCounter++, mpFirstElem, NULL, mManageContent);
	if (mpFirstElem) {
		mpFirstElem->setPrevious(pNewElem);
	};
	mpFirstElem = pNewElem;
	if (!mpLastElem) { mpLastElem = pNewElem; };
	mLength++;
	*pID = pNewElem->getID();
	return S_OK;
};

template<class ObjectType,class ListElemType>
inline HRESULT List_Template<ObjectType, ListElemType>::deleteAllListElements()
{
	V_RETURN(access());
	while (mLength > 0) {
		deleteListElement(mpFirstElem);
	}
	return S_OK;
}

template<class ObjectType>
class List : public List_Template<ObjectType, ListElement<ObjectType> >, public MemManaged{
protected:
	virtual ~List() {};
public:
	HRESULT Release()override { V_RETURN(access()); delete this; return S_OK; };
	List() : List_Template< ObjectType, ListElement<ObjectType> >() {};
};

template<class ObjectType>
class UnManagedList : public List_Template<ObjectType, UnManagedListElement<ObjectType> > {
protected:
	virtual ~UnManagedList() {};
public:
	HRESULT Release() { V_RETURN(access()); delete this; return S_OK; };
	UnManagedList() : List_Template< ObjectType, UnManagedListElement<ObjectType> >() {};
};

#endif