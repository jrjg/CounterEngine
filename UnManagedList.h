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
	ObjectType* pop();
	UnManagedListElement<ObjectType>* getLast() { return mpLastElem; };
	ObjectType* getByID(ID id);
	HRESULT deleteByID(ID id);
	HRESULT restore();
	void release() { delete this; };
	UnManagedList() : mpFirstElem(NULL),mpLastElem(NULL),mLength(0),mElemIDCounter(0),mManageContent(true) { restore(); };
};

template<class ObjectType>
inline HRESULT UnManagedList<ObjectType>::deleteByID(ID id)
{
	if (mLength > 0) {
		for (UnManagedListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == id) {
				pElem->release();
				return S_OK;
			}
		}
	}
	return S_OK;
};

template<class ObjectType>
inline HRESULT UnManagedList<ObjectType>::restore()
{
	if (mLength > 0) {
		for (UnManagedListElement<ObjectType>* pListElem = getFirst(); pListElem != NULL; pListElem = pListElem->getNext()) {
			pListElem->release();
		}
	}
	mpFirstElem = NULL;
	mpLastElem = NULL;
	mLength = 0;
	mElemIDCounter = 0;
	mManageContent = true;
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
ObjectType* UnManagedList<ObjectType>::pop()
{
	if (mLength == 0) { return NULL; };
	ObjectType* pObj = mpFirstElem->getObject();
	mpFirstElem->setDeleteContent(false); //dont delete what is returned
	mpFirstElem->release();
	return pObj;
};

template <class ObjectType>
ID UnManagedList<ObjectType>::pushBack(ObjectType* pObject)
{
	UnManagedListElement<ObjectType>* pNewElem = new UnManagedListElement<ObjectType>(this, pObject, mElemIDCounter++, NULL, mpLastElem, mManageContent);
	return pNewElem->getID();
};

template <class ObjectType>
ID UnManagedList<ObjectType>::pushFront(ObjectType* pObject)
{
	UnManagedListElement<ObjectType>* pNewElem = new UnManagedListElement<ObjectType>(this, pObject, mElemIDCounter++, mpFirstElem, NULL, mManageContent);
	return pNewElem->getID();
};

template <class ObjectType>
UnManagedList<ObjectType>::~UnManagedList() {
	if (mLength > 0) {
		for (UnManagedListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			pElem->release();
		}
	}
	return;
};

#endif