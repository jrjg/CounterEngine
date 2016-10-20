#ifndef UNMANAGEDLIST_INC
#define UNMANAGEDLIST_INC

#include "UnManagedListElement.h"

template <class ObjectType>
class UnManagedList {
private:
	UnManagedListElement<ObjectType>* mpFirstElem;
	UnManagedListElement<ObjectType>* mpLastElem;
	UINT mLength;
	ID mElemIDCounter;
	bool mManageContent;
	HRESULT deleteAllListElements();
	HRESULT deleteListElement(UnManagedListElement<ObjectType>* pListElem);
protected:
	virtual ~UnManagedList();
public:
	ID pushBack(ObjectType* pObj);
	ID pushFront(ObjectType* pObj);
	void setLength(unsigned int length) { mLength = length; };
	HRESULT setFirst(UnManagedListElement<ObjectType>* pElem) { mpFirstElem = pElem; return S_OK; };
	HRESULT setLast(UnManagedListElement<ObjectType>* pElem) { mpLastElem = pElem; return S_OK; };
	UINT getLength() { return mLength; };
	UnManagedListElement<ObjectType>* getFirst() { return mpFirstElem; };
	ObjectType* popFirst();
	ObjectType* popLast();
	ObjectType* pop(ID id);
	UnManagedListElement<ObjectType>* getLast() { return mpLastElem; };
	ObjectType* getByID(ID id);
	HRESULT deleteByID(ID id);
	HRESULT restore();
	UnManagedList() : mpFirstElem(NULL), mpLastElem(NULL), mLength(0), mElemIDCounter(0), mManageContent(true) { restore(); };
	UnManagedList(bool manageContent) : mpFirstElem(NULL), mpLastElem(NULL), mLength(0), mElemIDCounter(0), mManageContent(manageContent) { restore(); };
	HRESULT Release() { delete this; return S_OK; };
};

template<class ObjectType>
inline ObjectType * UnManagedList<ObjectType>::pop(ID id)
{
	ObjectType* pObj = nullptr;
	if (mLength > 0) {
		for (UnManagedListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == id) {
				pObj = pElem->getObject();
				pElem->setDeleteContent(false); //dont delete what is returned
				deleteListElement(pElem);
				break;
			}
		}
	}
	return pObj;
};

template<class ObjectType>
inline HRESULT UnManagedList<ObjectType>::deleteByID(ID id)
{
	if (mLength > 0) {
		for (UnManagedListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == id) {
				return deleteListElement(pElem);
			}
		}
	}
	return S_OK;
};

template<class ObjectType>
inline HRESULT UnManagedList<ObjectType>::restore()
{
	deleteAllListElements();
	mpFirstElem = NULL;
	mpLastElem = NULL;
	mLength = 0;
	mElemIDCounter = 0;
	mManageContent = true;
	return S_OK;
};

template<class ObjectType>
inline HRESULT UnManagedList<ObjectType>::deleteListElement(UnManagedListElement<ObjectType>* pListElem)
{
	if (mpFirstElem->getID() == pListElem->getID()) { mpFirstElem = pListElem->getNext(); };
	if (mpLastElem->getID() == pListElem->getID()) { mpLastElem = pListElem->getPrevious(); };
	if (pListElem->getNext()) { pListElem->getNext()->setPrevious(pListElem->getPrevious()); };
	if (pListElem->getPrevious()) { pListElem->getPrevious()->setNext(pListElem->getNext()); };
	mLength--;
	SAFE_RELEASE(pListElem);
	return S_OK;
};

template <class ObjectType>
ObjectType* UnManagedList<ObjectType>::getByID(ID id) {
	ObjectType* pObj = nullptr;
	if (mLength > 0) {
		for (UnManagedListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == id) {
				pObj = pElem->getObject();
				return pObj;
			}
		}
	}
	return pObj;
};

template <class ObjectType>
ObjectType* UnManagedList<ObjectType>::popFirst()
{
	ObjectType* pObj;
	if (mpFirstElem) {
		pObj = mpFirstElem->getObject();
		mpFirstElem->setDeleteContent(false); //dont delete what is returned
		deleteListElement(mpFirstElem);
	}
	return pObj;
};

template <class ObjectType>
ObjectType* UnManagedList<ObjectType>::popLast()
{
	ObjectType* pObj;
	if (mpLastElem) {
		pObj = mpLastElem->getObject();
		mpLastElem->setDeleteContent(false); //dont delete what is returned
		deleteListElement(mpLastElem);
	}
	return pObj;
};

template <class ObjectType>
ID UnManagedList<ObjectType>::pushBack(ObjectType* pObject)
{
	UnManagedListElement<ObjectType>* pNewElem = new UnManagedListElement<ObjectType>(pObject, mElemIDCounter++, NULL, mpLastElem, mManageContent);
	if (mpLastElem) { 
		mpLastElem->setNext(pNewElem); 
	};
	mpLastElem = pNewElem;
	if (!mpFirstElem) { mpFirstElem = pNewElem; };
	mLength++;
	return pNewElem->getID();
};

template <class ObjectType>
ID UnManagedList<ObjectType>::pushFront(ObjectType* pObject)
{
	UnManagedListElement<ObjectType>* pNewElem = new UnManagedListElement<ObjectType>(pObject, mElemIDCounter++, mpFirstElem, NULL, mManageContent);
	if (mpFirstElem) {
		mpFirstElem->setPrevious(pNewElem);
	};
	mpFirstElem = pNewElem;
	if (!mpLastElem) { mpLastElem = pNewElem; };
	mLength++;
	return pNewElem->getID();
};

template<class ObjectType>
inline HRESULT UnManagedList<ObjectType>::deleteAllListElements()
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

template <class ObjectType>
UnManagedList<ObjectType>::~UnManagedList() {
	deleteAllListElements();
	return; 
};

#endif