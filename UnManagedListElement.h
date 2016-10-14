#ifndef UNMANAGEDLISTELEMENT
#define UNMANAGEDLISTELEMENT

#include "MemManaged.h"

template<class ObjectType>class UnManagedList;

template <class ObjectType>
class UnManagedListElement
{
private:
	UnManagedListElement<ObjectType>* mpPrevious;
	UnManagedListElement<ObjectType>* mpNext;
	ObjectType* mpObject;
	ID mID;
	bool mDeleteContent;
	UnManagedList<ObjectType>* mpList;
protected:
	virtual ~UnManagedListElement();
public:
	UnManagedListElement(UnManagedList<ObjectType>* pList, ObjectType* pObj, ID id, UnManagedListElement<ObjectType>* pNext, UnManagedListElement<ObjectType>* pPrevious, bool memManageContent);
	ID getID() { return mID; };
	UnManagedListElement<ObjectType>* getNext() { return mpNext; };
	UnManagedListElement<ObjectType>* getPrevious() { return mpPrevious; };
	void setNext(UnManagedListElement<ObjectType>* p) { mpNext = p; };
	void setPrevious(UnManagedListElement<ObjectType>* p) { mpPrevious = p; };
	ObjectType* getObject() { return mpObject; };
	void setDeleteContent(bool b) { mDeleteContent = b; };
	void Release() { delete this; };
};

template<class ObjectType>
inline UnManagedListElement<ObjectType>::UnManagedListElement(UnManagedList<ObjectType>* pList, ObjectType * pObj, ID id, UnManagedListElement<ObjectType>* pNext, UnManagedListElement<ObjectType>* pPrevious, bool memManageContent) : mpList(pList), mpObject(pObj), mID(id), mDeleteContent(memManageContent), mpNext(pNext), mpPrevious(pPrevious)
{
	if (!pNext) { //pushback
		if (pPrevious) { pPrevious->setNext(this); };
		mpList->setLast(this);
	}
	if (!pPrevious) { //pushfront
		if (pNext) { pNext->setPrevious(this); };
		mpList->setFirst(this);
	}
	mpList->setLength(mpList->getLength() + 1);
};

template <class ObjectType> UnManagedListElement<ObjectType>::~UnManagedListElement()
{
	if (!mpList) { if (mDeleteContent) { SAFE_RELEASE(mpObject); }; return; };
	if (mpList->getLength() == 1) {
		mpList->setFirst(NULL);
		mpList->setLast(NULL);
	}
	else {
		if (mpNext) { 
			mpNext->setPrevious(NULL); 
		}
		else {
			mpList->setLast(mpPrevious);
		};
		if (mpPrevious) { 
			mpPrevious->setNext(NULL); 
		}
		else {
			mpList->setFirst(mpNext);
		};
	}
	mpList->setLength(mpList->getLength() - 1);
	if (mDeleteContent) { SAFE_RELEASE(mpObject); };
	return;
};

#endif