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
				SAFE_RELEASE(pElem);
				return S_OK;
			}
		}
	}
	return S_OK;
};

template<class ObjectType>
inline HRESULT List<ObjectType>::restore()
{
	if (mLength > 0) {
		for (ListElement<ObjectType>* pListElem = getFirst(); pListElem != NULL; pListElem = pListElem->getNext()) {
			SAFE_RELEASE(pListElem);
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
		SAFE_RELEASE(mpFirstElem);
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
		SAFE_RELEASE(mpLastElem);
	}
	return pObj;
};

template <class ObjectType>
ID List<ObjectType>::pushBack(ObjectType* pObject)
{
	ListElement<ObjectType>* pNewElem = new ListElement<ObjectType>(this, pObject, mElemIDCounter++, NULL, mpLastElem, mManageContent);
	return pNewElem->getID();
};

template <class ObjectType>
ID List<ObjectType>::pushFront(ObjectType* pObject)
{
	ListElement<ObjectType>* pNewElem = new ListElement<ObjectType>(this, pObject, mElemIDCounter++, mpFirstElem, NULL, mManageContent);
	return pNewElem->getID();
};

template <class ObjectType>
List<ObjectType>::~List() {
	if (mLength > 0) {
		for (ListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
			SAFE_RELEASE(pElem);
		}
	}
	return;
};

#endif