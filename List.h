#ifndef INCLUDE_LIST
#define INCLUDE_LIST

#include "TopInclude.h"
#include "MemManaged.h"

template <class ObjectType>
class UnManagedListElement
{
protected:
	UnManagedListElement<ObjectType>* mpPrevious;
	UnManagedListElement<ObjectType>* mpNext;
	ObjectType* mpObject;
	ID mID;
	bool mDeleteContent;
public:
	virtual ~UnManagedListElement() { if (mDeleteContent) { SAFE_RELEASE(mpObject); }; };
	UnManagedListElement(ObjectType* pObj, ID id, UnManagedListElement<ObjectType>* pNext, UnManagedListElement<ObjectType>* pPrevious, bool memManageContent) : mpObject(pObj), mID(id), mDeleteContent(memManageContent), mpNext(pNext), mpPrevious(pPrevious) {};
	ID getID() { return mID; };
	UnManagedListElement<ObjectType>* getNext() { return mpNext; };
	UnManagedListElement<ObjectType>* getPrevious() { return mpPrevious; };
	void setNext(UnManagedListElement<ObjectType>* p) { mpNext = p; };
	void setPrevious(UnManagedListElement<ObjectType>* p) { mpPrevious = p; };
	ObjectType* getObject() { return mpObject; };
	void setDeleteContent(bool b) { mDeleteContent = b; };
	bool getDeleteContent() { return mDeleteContent; };
	void Release() { delete this; };
};

template <class ObjectType>
class ListElement : public UnManagedListElement<ObjectType>, public MemManaged {
public:
	void Release() override{ delete this; };
	virtual ~ListElement() {};
	ListElement(ObjectType* pObj, ID id, UnManagedListElement<ObjectType>* pNext, UnManagedListElement<ObjectType>* pPrevious, bool memManageContent) : UnManagedListElement<ObjectType>(pObj, id, pNext, pPrevious, memManageContent) {};
};

template <class ObjectType, class ListElemType>
class List_Template{
protected:
	ListElemType* getLast() { return mpLastElem; };
	ListElemType* mpFirstElem;
	ListElemType* mpLastElem;
	UINT mLength;
	ID mElemIDCounter;
	bool mManageContent;
	HRESULT deleteAllListElements();
	HRESULT deleteListElement(ListElemType* pListElem);
	ObjectType* pop(ListElemType* pElem);
public:
	ID pushBack(ObjectType* pObj);
	ID pushFront(ObjectType* pObj);
	void setLength(unsigned int length) { mLength = length; };
	HRESULT setFirst(ListElemType* pElem) { mpFirstElem = pElem; return S_OK; };
	HRESULT setLast(ListElemType* pElem) { mpLastElem = pElem; return S_OK; };
	UINT getLength() { return mLength; };
	ListElemType* iterator() { return mpFirstElem; };
	ObjectType* popFirst() { return pop(mpFirstElem); };
	ObjectType* popLast() { return pop(mpLastElem); };
	ObjectType* pop(ID id);
	
	ObjectType* getByID(ID id);
	HRESULT deleteByID(ID id);
	HRESULT restore();
	virtual ~List_Template();
	List_Template() : mpFirstElem(NULL), mpLastElem(NULL), mLength(0), mElemIDCounter(0), mManageContent(true) { restore(); };
};

template<class ObjectType,class ListElemType>
inline HRESULT List_Template<ObjectType, ListElemType>::restore()
{
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
	if (pListElem) {
		if (mpFirstElem->getID() == pListElem->getID()) { mpFirstElem = (ListElemType*)pListElem->getNext(); };
		if (mpLastElem->getID() == pListElem->getID()) { mpLastElem = (ListElemType*)pListElem->getPrevious(); };
		if (pListElem->getNext()) { pListElem->getNext()->setPrevious((ListElemType*)pListElem->getPrevious()); };
		if (pListElem->getPrevious()) { pListElem->getPrevious()->setNext((ListElemType*)pListElem->getNext()); };
		mLength--;
		SAFE_RELEASE(pListElem);
	}
	return S_OK;
};

template <class ObjectType,class ListElemType>
ObjectType* List_Template<ObjectType, ListElemType>::getByID(ID id) {
	ObjectType* pObj = nullptr;
	if (mLength > 0) {
		for (ListElemType* pElem = (ListElemType*)mpFirstElem; pElem != NULL; pElem = (ListElemType*)pElem->getNext()) {
			if (pElem->getID() == id) {
				pObj = pElem->getObject();
				return pObj;
			}
		}
	}
	return pObj;
}
template<class ObjectType, class ListElemType>
inline HRESULT List_Template<ObjectType, ListElemType>::deleteByID(ID id)
{
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
inline ObjectType * List_Template<ObjectType, ListElemType>::pop(ID id)
{
	if (mLength > 0) {
		for (ListElemType* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == id) {
				return pop(pElem);
			}
		}
	}
	return NULL;
}
template<class ObjectType,class ListElemType>
inline ObjectType * List_Template<ObjectType, ListElemType>::pop(ListElemType* pElem)
{
	ObjectType* pObj;
	if (pElem) {
		pObj = pElem->getObject();
		pElem->setDeleteContent(false); //dont delete what is returned
		deleteListElement(pElem);
	}
	
	return pObj;
};

template <class ObjectType,class ListElemType>
ID List_Template<ObjectType, ListElemType>::pushBack(ObjectType* pObject)
{
	ListElemType* pNewElem = new ListElemType(pObject, mElemIDCounter++, NULL, mpLastElem, mManageContent);
	if (mpLastElem) {
		mpLastElem->setNext(pNewElem);
	};
	mpLastElem = pNewElem;
	if (!mpFirstElem) { mpFirstElem = pNewElem; };
	mLength++;
	return pNewElem->getID();
};

template <class ObjectType,class ListElemType>
ID List_Template<ObjectType, ListElemType>::pushFront(ObjectType* pObject)
{
	ListElemType* pNewElem = new ListElemType(pObject, mElemIDCounter++, mpFirstElem, NULL, mManageContent);
	if (mpFirstElem) {
		mpFirstElem->setPrevious(pNewElem);
	};
	mpFirstElem = pNewElem;
	if (!mpLastElem) { mpLastElem = pNewElem; };
	mLength++;
	return pNewElem->getID();
};

template<class ObjectType,class ListElemType>
inline HRESULT List_Template<ObjectType, ListElemType>::deleteAllListElements()
{
	while (mLength > 0) {
		//V_RETURN(deleteListElement(mpFirstElem));
		deleteListElement(mpFirstElem);
	}
	return S_OK;
}

template <class ObjectType,class ListElemType>
List_Template<ObjectType, ListElemType>::~List_Template() {
	deleteAllListElements();
	return;
};

template<class ObjectType>
class List : public MemManaged, public List_Template<ObjectType, ListElement<ObjectType> >{
public:
	virtual ~List() {};
	List() : List_Template<ObjectType, ListElement<ObjectType>>() {};
};

template<class ObjectType>
class UnManagedList : public List_Template<ObjectType, UnManagedListElement<ObjectType> > {
public:
	void Release() { delete this; };
	virtual ~UnManagedList() {};
	UnManagedList() : List_Template<ObjectType, UnManagedListElement<ObjectType>>() {};
};

#endif