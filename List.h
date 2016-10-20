#ifndef INCLUDE_LIST
#define INCLUDE_LIST

#include "ListElement.h"
#include "MemManaged.h"

template <class ObjectType>
class List : public MemManaged {
private:
	ListElement<ObjectType>* mpFirstElem;
	ListElement<ObjectType>* mpLastElem;
	UINT mLength;
	ID mElemIDCounter;
	bool mManageContent;
	HRESULT deleteAllListElements();
	HRESULT deleteListElement(ListElement<ObjectType>* pListElem);
protected:
	virtual ~List();
public:
	ID pushBack(ObjectType* pObj);
	ID pushFront(ObjectType* pObj);
	void setLength(unsigned int length) { mLength = length; };
	HRESULT setFirst(ListElement<ObjectType>* pElem) { mpFirstElem = pElem; return S_OK; };
	HRESULT setLast(ListElement<ObjectType>* pElem) { mpLastElem = pElem; return S_OK; };
	UINT getLength() { return mLength; };
	ListElement<ObjectType>* getFirst() { return mpFirstElem; };
	ObjectType* popFirst();
	ObjectType* popLast();
	ObjectType* pop(ID id);
	ListElement<ObjectType>* getLast() { return mpLastElem; };
	ObjectType* getByID(ID id);
	HRESULT deleteByID(ID id);
	HRESULT restore();
	List() : mpFirstElem(NULL), mpLastElem(NULL), mLength(0), mElemIDCounter(0), mManageContent(true) { restore(); };
};

template<class ObjectType>
inline HRESULT List<ObjectType>::deleteByID(ID id)
{
	if (mLength > 0) {
		for (ListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == id) {
				return deleteListElement(pElem);
			}
		}
	}
	return S_OK;
};

template<class ObjectType>
inline HRESULT List<ObjectType>::restore()
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
inline HRESULT List<ObjectType>::deleteListElement(ListElement<ObjectType>* pListElem)
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
ObjectType* List<ObjectType>::getByID(ID id) {
	ObjectType* pObj = nullptr;
	if (mLength > 0) {
		for (ListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			if (pElem->getID() == id) {
				pObj = pElem->getObject();
				return pObj;
			}
		}
	}
	return pObj;
};

template <class ObjectType>
ObjectType* List<ObjectType>::popFirst()
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
ObjectType* List<ObjectType>::popLast()
{
	ObjectType* pObj;
	if (mpLastElem) {
		pObj = mpLastElem->getObject();
		mpLastElem->setDeleteContent(false); //dont delete what is returned
		deleteListElement(mpLastElem);
	}
	return pObj;
}

template<class ObjectType>
inline ObjectType * List<ObjectType>::pop(ID id)
{
	ObjectType* pObj = nullptr;
	if (mLength > 0) {
		for (ListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
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

template <class ObjectType>
ID List<ObjectType>::pushBack(ObjectType* pObject)
{
	ListElement<ObjectType>* pNewElem = new ListElement<ObjectType>(pObject, mElemIDCounter++, NULL, mpLastElem, mManageContent);
	if (mpLastElem) {
		mpLastElem->setNext(pNewElem);
	};
	mpLastElem = pNewElem;
	if (!mpFirstElem) { mpFirstElem = pNewElem; };
	mLength++;
	return pNewElem->getID();
};

template <class ObjectType>
ID List<ObjectType>::pushFront(ObjectType* pObject)
{
	ListElement<ObjectType>* pNewElem = new ListElement<ObjectType>(pObject, mElemIDCounter++, mpFirstElem, NULL, mManageContent);
	if (mpFirstElem) {
		mpFirstElem->setPrevious(pNewElem);
	};
	mpFirstElem = pNewElem;
	if (!mpLastElem) { mpLastElem = pNewElem; };
	mLength++;
	return pNewElem->getID();
};

template<class ObjectType>
inline HRESULT List<ObjectType>::deleteAllListElements()
{
	ObjectType* pObject;
	while (mLength > 0) {
		bool manageContent = mpFirstElem->
		pObject = popFirst();
		if (mManageContent) {
			SAFE_RELEASE(pObject);
		}
	}
	return S_OK;
}

template <class ObjectType>
List<ObjectType>::~List() {
	deleteAllListElements();
	return;
};

#endif