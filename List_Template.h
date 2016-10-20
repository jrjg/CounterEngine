#ifndef INCLUDE_LIST_TEMPLATE
#define INCLUDE_LIST_TEMPLATE

#include "ListElement.h"
#include "UnManagedListElement.h"
#include "MemManaged.h"

template<class ListElement>class ListElemType;
template<class UnManagedListElement>class ListElemType;

template <class ObjectType,class ListElemType>
class List_Template : public MemManaged {
private:
	ListElemType* mpFirstElem;
	ListElemType* mpLastElem;
	UINT mLength;
	ID mElemIDCounter;
	bool mManageContent;
	HRESULT deleteAllListElements();
	HRESULT deleteListElement(ListElemType* pListElem);
protected:
	virtual ~List_Template();
public:
	ID pushBack(ObjectType* pObj);
	ID pushFront(ObjectType* pObj);
	void setLength(unsigned int length) { mLength = length; };
	HRESULT setFirst(ListElemType* pElem) { mpFirstElem = pElem; return S_OK; };
	HRESULT setLast(ListElemType* pElem) { mpLastElem = pElem; return S_OK; };
	UINT getLength() { return mLength; };
	ListElemType* getFirst() { return mpFirstElem; };
	ObjectType* popFirst();
	ObjectType* popLast();
	ObjectType* pop(ID id);
	ListElemType* getLast() { return mpLastElem; };
	ObjectType* getByID(ID id);
	HRESULT deleteByID(ID id);
	HRESULT restore();
	List_Template() : mpFirstElem(NULL), mpLastElem(NULL), mLength(0), mElemIDCounter(0), mManageContent(true) { restore(); };
};

template<class ObjectType,class ListElemType>
inline HRESULT List_Template<ObjectType, ListElemType>::deleteByID(ID id)
{
	if (mLength > 0) {
		for (ListElemType* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == id) {
				return deleteListElement(pElem);
			}
		}
	}
	return S_OK;
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
	if (mpFirstElem->getID() == pListElem->getID()) { mpFirstElem = pListElem->getNext(); };
	if (mpLastElem->getID() == pListElem->getID()) { mpLastElem = pListElem->getPrevious(); };
	if (pListElem->getNext()) { pListElem->getNext()->setPrevious(pListElem->getPrevious()); };
	if (pListElem->getPrevious()) { pListElem->getPrevious()->setNext(pListElem->getNext()); };
	mLength--;
	SAFE_RELEASE(pListElem);
	return S_OK;
};

template <class ObjectType, class ListElemType>
ObjectType* List_Template<ObjectType, ListElemType>::getByID(ID id) {
	ObjectType* pObj = nullptr;
	if (mLength > 0) {
		for (ListElemType* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == id) {
				pObj = pElem->getObject();
				return pObj;
			}
		}
	}
	return pObj;
};

template <class ObjectType, class ListElemType>
ObjectType* List_Template<ObjectType, ListElemType>::popFirst()
{
	ObjectType* pObj;
	if (mpFirstElem) {
		pObj = mpFirstElem->getObject();
		mpFirstElem->setDeleteContent(false); //dont delete what is returned
		deleteListElement(mpFirstElem);
	}
	return pObj;
};

template <class ObjectType, class ListElemType>
ObjectType* List_Template<ObjectType, ListElemType>::popLast()
{
	ObjectType* pObj;
	if (mpLastElem) {
		pObj = mpLastElem->getObject();
		mpLastElem->setDeleteContent(false); //dont delete what is returned
		deleteListElement(mpLastElem);
	}
	return pObj;
}

template<class ObjectType, class ListElemType>
inline ObjectType * List_Template<ObjectType, ListElemType>::pop(ID id)
{
	ObjectType* pObj = nullptr;
	if (mLength > 0) {
		for (ListElemType* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == id) {
				pObj = mpLastElem->getObject();
				mpLastElem->setDeleteContent(false); //dont delete what is returned
				deleteListElement(mpLastElem);
			}
		}
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
	ObjectType* pObject;
	while (mLength > 0) {
		pObject = popFirst();
		if (mManageContent) {
			delete pObject;
		}
	}
	return S_OK;
}

template<class ObjectType, class ListElemType>
inline List_Template<ObjectType, ListElemType>::~List_Template()
{
	deleteAllListElements();
	return;
}

#endif